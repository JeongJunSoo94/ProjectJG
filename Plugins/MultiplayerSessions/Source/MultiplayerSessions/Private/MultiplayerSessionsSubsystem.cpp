#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/Texture2D.h"
#include "ThirdParty/Steamworks/Steamv151/sdk/public/steam/steam_api.h"
#include "ThirdParty/Steamworks/Steamv151/sdk/public/steam/isteamuser.h"
#include "ThirdParty/Steamworks/Steamv151/sdk/public/steam/isteamutils.h"


#include "GameFramework/PlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() :
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface)
		{
			SessionInterface->OnSessionUserInviteAcceptedDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnInviteAccepted);
		}
	}
}

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	DesiredNumPublicConnections = NumPublicConnections;
	DesiredMatchType = MatchType;
	if (!SessionInterface.IsValid())
	{
		return;
	}

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = NumPublicConnections;
		LastMatchType = MatchType;

		DestroySession();
	}

	// Store the delegate in a FDelegateHandle so we can later remove it from the delegate list
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->BuildUniqueId = 1;
	LastSessionSettings->bUseLobbiesIfAvailable = true;

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// Broadcast our own custom delegate
		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::CreateSession(FString SessionName,int32 NumPublicConnections, FString MatchType)
{
	DesiredNumPublicConnections = NumPublicConnections;
	DesiredMatchType = MatchType;
	if (!SessionInterface.IsValid())
	{
		return;
	}

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = NumPublicConnections;
		LastMatchType = MatchType;

		DestroySession();
	}

	// Store the delegate in a FDelegateHandle so we can later remove it from the delegate list
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->Set(FName("SESSION_LOBBY_NAME"), SessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->BuildUniqueId = 1;
	LastSessionSettings->bUseLobbiesIfAvailable = true;

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// Broadcast our own custom delegate
		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	UWorld* world = GetWorld();
	if (world)
	{
		const ULocalPlayer* LocalPlayer = world->GetFirstLocalPlayerFromController();
		if(LocalPlayer)
			UE_LOG(LogTemp, Log, TEXT("LocalPlayerGetFName %s"), *LocalPlayer->GetFName().ToString());
		if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
		{
			SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
			UE_LOG(LogTemp, Log, TEXT("Find MultiplayerOnFindSessionsComplete."));
			MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		}
	}
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	UWorld* world = GetWorld();
	if (world)
	{
		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
		{
			SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
			UE_LOG(LogTemp, Log, TEXT("Find MultiplayerOnJoinSessionComplete."));
			MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		}
	}
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(NAME_GameSession))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		MultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::StartSession()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->StartSession(NAME_GameSession);
		}
	}
}

FName UMultiplayerSessionsSubsystem::GetSessionName()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		if (Sessions.IsValid())
		{
			FNamedOnlineSession* CurrentSession = Sessions->GetNamedSession(NAME_GameSession);
			if (CurrentSession)
			{
				return CurrentSession->SessionName;
			}
		}
	}

	return NAME_None;
}

FString UMultiplayerSessionsSubsystem::GetSessionOwnerName()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		if (Sessions.IsValid())
		{
			FNamedOnlineSession* CurrentSession = Sessions->GetNamedSession(NAME_GameSession);
			if (CurrentSession)
			{
				return CurrentSession->OwningUserName;
			}
		}
	}

	return "NAME_None";
}

int32 UMultiplayerSessionsSubsystem::GetSessionPlayerNum()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		if (Sessions.IsValid())
		{
			FNamedOnlineSession* CurrentSession = Sessions->GetNamedSession(NAME_GameSession);
			if (CurrentSession)
			{
				return CurrentSession->SessionSettings.NumPublicConnections;
			}
		}
	}

	return -1;
}

FString UMultiplayerSessionsSubsystem::GetSessionLobbyName()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		if (Sessions.IsValid())
		{
			FNamedOnlineSession* CurrentSession = Sessions->GetNamedSession(NAME_GameSession);

			if (CurrentSession)
			{
				FString SessionName;
				if (CurrentSession->SessionSettings.Get(FName("SESSION_LOBBY_NAME"), SessionName))
				{
					return SessionName;
				}
			}
		}
	}

	return "NAME_None";
}

FName UMultiplayerSessionsSubsystem::GetOnlineSubsystemName()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		return FName(OnlineSubsystem->GetSubsystemName());
	}
	return NAME_None;
}

const TArray<FOnlineSessionSearchResult> UMultiplayerSessionsSubsystem::GetSearchResults()
{ 
	return LastSessionSearch->SearchResults;
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	MultiplayerOnJoinSessionComplete.Broadcast(Result);
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
	if (bWasSuccessful && bCreateSessionOnDestroy)
	{
		bCreateSessionOnDestroy = false;
		CreateSession(LastNumPublicConnections, LastMatchType);
	}
	MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		//SessionInterface->StartSession(SessionName);
	}
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		FriendsInterface = Subsystem->GetFriendsInterface();

		if (FriendsInterface.IsValid())
		{
			FetchFriendsList();
		}
	}
}


void UMultiplayerSessionsSubsystem::FetchFriendsList()
{
	if (FriendsInterface.IsValid())
	{
		FriendsInterface->ReadFriendsList(0, TEXT("Default"), FOnReadFriendsListComplete::CreateLambda([this](int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
			{
				if (bWasSuccessful)
				{
					UE_LOG(LogTemp, Log, TEXT("FriendsInterfaceSuccess"));
					FriendsList.Empty();
					if (FriendsInterface->GetFriendsList(LocalUserNum, ListName, FriendsList))
					{
						UE_LOG(LogTemp, Log, TEXT("FriendsListSuccess"));
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("FriendsListFail"));
					}

				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("FriendsInterface Fail: %s"), *ErrorStr);
				}
			}));
	}
}


UTexture2D* UMultiplayerSessionsSubsystem::GetSteamFriendAvatar(const uint64 UniqueNetId)  //, PlayerAvatarSize PlayerAvatarSize)
{

	uint32 Width = 0;
	uint32 Height = 0;

	if (SteamAPI_Init())
	{
		int Picture = 0;

		//switch (PlayerAvatarSize)
		//{
		//case PlayerAvatarSize::PlayerAvatar_Small: Picture = SteamFriends()->GetSmallFriendAvatar(UniqueNetId); break;
		//case PlayerAvatarSize::PlayerAvatar_Medium: Picture = SteamFriends()->GetMediumFriendAvatar(UniqueNetId); break;
		//case PlayerAvatarSize::PlayerAvatar_Large: Picture = SteamFriends()->GetLargeFriendAvatar(UniqueNetId); break;
		//default: break;
		//}

		Picture = SteamFriends()->GetMediumFriendAvatar(UniqueNetId);

		if (Picture == -1)
		{
			return NULL;
		}

		SteamUtils()->GetImageSize(Picture, &Width, &Height);

		if (Width > 0 && Height > 0)
		{
			//Creating the buffer "oAvatarRGBA" and then filling it with the RGBA Stream from the Steam Avatar
			uint8* oAvatarRGBA = new uint8[Width * Height * 4];


			//Filling the buffer with the RGBA Stream from the Steam Avatar and creating a UTextur2D to parse the RGBA Steam in
			SteamUtils()->GetImageRGBA(Picture, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));

			UTexture2D* Avatar = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
			// Switched to a Memcpy instead of byte by byte transer
			uint8* MipData = (uint8*)Avatar->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
			Avatar->PlatformData->Mips[0].BulkData.Unlock();

			// Original implementation was missing this!!
			// the hell man......
			delete[] oAvatarRGBA;

			//Setting some Parameters for the Texture and finally returning it
			Avatar->PlatformData->SetNumSlices(1);
			Avatar->NeverStream = true;
			//Avatar->CompressionSettings = TC_EditorIcon;

			Avatar->UpdateResource();

			//TextureCacheMap.Add(UniqueNetId, Avatar);
			return Avatar;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Bad Height / Width with steam avatar!"));
		}

		return nullptr;
	}
	return nullptr;
}

void UMultiplayerSessionsSubsystem::SendSessionInviteToFriend(int32 LocalUserNum,const FUniqueNetId& UniqueNetId)
{
	if (SessionInterface->SendSessionInviteToFriend(LocalUserNum, NAME_GameSession, UniqueNetId))
	{
		UE_LOG(LogTemp, Log, TEXT("Invite sent to friend!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to send invite to friend!"));
	}
}

void UMultiplayerSessionsSubsystem::OnInviteAccepted(bool bWasSuccessful, int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& SearchResult)
{
	if (bWasSuccessful && SearchResult.IsValid())
	{
		if (SessionInterface.IsValid())
		{
			JoinSession(SearchResult);

			UE_LOG(LogTemp, Log, TEXT("Invite acceptance Success."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invite acceptance failed."));
	}
}

void UMultiplayerSessionsSubsystem::InviteAcceptedLog(int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& SearchResult)
{
	UE_LOG(LogTemp, Log, TEXT("ControllerId: %d"), ControllerId);
	UE_LOG(LogTemp, Log, TEXT("UserId: %s"), *UserId->ToString());
	UE_LOG(LogTemp, Log, TEXT("SearchResult: %s"), *SearchResult.GetSessionIdStr());

	FString SessionName;
	SearchResult.Session.SessionSettings.Get(FName("SESSION_LOBBY_NAME"), SessionName);
	UE_LOG(LogTemp, Log, TEXT("SESSION_LOBBY_NAME: %s"),
		*SessionName);
	UE_LOG(LogTemp, Log, TEXT("SearchResult.Session.OwningUserId: %s"),
		*SearchResult.Session.OwningUserId.Get()->ToString());
	UE_LOG(LogTemp, Log, TEXT("SearchResult.Session.OwningUserName: %s"),
		*SearchResult.Session.OwningUserName);
	UE_LOG(LogTemp, Log, TEXT("SearchResult.Session.SessionInfo: %s"),
		*SearchResult.Session.SessionInfo.Get()->ToString());
}

