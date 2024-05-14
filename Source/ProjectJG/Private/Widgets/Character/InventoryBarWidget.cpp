#include "Widgets/Character/InventoryBarWidget.h"
#include "Widgets/Character/ItemSlotWidget.h"
#include "WorldObjects/Item/Item.h"


//void UInventoryBarWidget::StartIconAnim()
//{}

UItemSlotWidget* UInventoryBarWidget::GetSlotWidget(int32 Index)
{
	switch (Index)
	{
		case 0:
			return WeaponSlot1;
		case 1:
			return WeaponSlot2;
		case 2:
			return WeaponSlot3;
		case 3:
			return WeaponSlot4;
		case 4:
			return WeaponSlot5;
	}
	return nullptr;
}