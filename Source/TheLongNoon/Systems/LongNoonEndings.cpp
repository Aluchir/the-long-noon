#include "Systems/LongNoonEndings.h"

ELongNoonEnding ULongNoonEndingLibrary::ResolveEnding(ELongNoonChoice Choice, bool bRemembererFound)
{
	switch (Choice)
	{
	case ELongNoonChoice::GiveItBack:
		return ELongNoonEnding::GiftReturned;
	case ELongNoonChoice::KeepTheNoon:
		return ELongNoonEnding::LongNoonContinues;
	case ELongNoonChoice::TheThirdWay:
		// Only the player who found the Rememberer can choose to return memory.
		return bRemembererFound ? ELongNoonEnding::ThirdWay : ELongNoonEnding::None;
	default:
		return ELongNoonEnding::None;
	}
}
