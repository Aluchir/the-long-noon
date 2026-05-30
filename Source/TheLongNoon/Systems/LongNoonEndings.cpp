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

FText ULongNoonEndingLibrary::GetEndingTitle(ELongNoonEnding Ending)
{
	switch (Ending)
	{
	case ELongNoonEnding::GiftReturned:
		return NSLOCTEXT("LongNoon", "EndGiftTitle", "The Gift Returned");
	case ELongNoonEnding::LongNoonContinues:
		return NSLOCTEXT("LongNoon", "EndKeepTitle", "The Long Noon Continues");
	case ELongNoonEnding::ThirdWay:
		return NSLOCTEXT("LongNoon", "EndThirdTitle", "The Third Way");
	default:
		return NSLOCTEXT("LongNoon", "EndNoneTitle", "Not yet");
	}
}

FText ULongNoonEndingLibrary::GetEndingText(ELongNoonEnding Ending)
{
	switch (Ending)
	{
	case ELongNoonEnding::GiftReturned:
		return NSLOCTEXT("LongNoon", "EndGiftText",
			"You ask for the weight back. The Gardener does not argue; it is almost relieved to be told. "
			"The sun tips toward evening, and a real night comes, frightening and beautiful and meaning something. "
			"People begin to age. Somewhere, eventually, someone is born. It will hurt now. It will also, finally, matter.");
	case ELongNoonEnding::LongNoonContinues:
		return NSLOCTEXT("LongNoon", "EndKeepText",
			"You cannot do it. The cost of giving death back is too cruel to choose for everyone. "
			"The light does not change. The Sunhollow stays warm and lovely forever, and everyone stays kind, "
			"and incurious, and untethered, never knowing what they lost. You stand in the golden meadow that "
			"started it all, the only one who remembers, and who chose.");
	case ELongNoonEnding::ThirdWay:
		return NSLOCTEXT("LongNoon", "EndThirdText",
			"You offer the Gardener a thing it never imagined: do not force death back, and do not keep the "
			"forgetting. Give the world its memory, and let each person choose, one at a time, whether to remember. "
			"Memory returns gently, like a tide. Some choose to remember, and weep, and hold each other differently. "
			"Some are not ready, and that is allowed. For the first time in an age, no one who feels is alone.");
	default:
		return NSLOCTEXT("LongNoon", "EndNoneText",
			"You sense a third shape to the answer, but it will not come. Something is still missing from you. "
			"Someone in the Hush is still waiting to be remembered.");
	}
}
