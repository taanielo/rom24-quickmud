#if defined(macintosh)
#include <types.h>
#include <time.h>
#else

#endif

#include <stdlib.h>
#include "merc.h"
#include "magic.h"

void do_identify(CHAR_DATA *ch, char *argument) {
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *mob;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	int cost, sn;
	OBJ_DATA *obj;

	/* check for sage */
	for (mob = ch->in_room->people; mob; mob = mob->next_in_room) {
		if (IS_NPC(mob) && IS_SET(mob->act, ACT_IS_SAGE)) {
			break;
		}
	}

	if (mob == NULL) {
		send_to_char("You can't do that here.\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg1);
	one_argument(argument, arg2);

	// item not provided
	if (arg1[0] == '\0') {
		send_to_char("Identify what?\n\r", ch);
		return;
	}

	if (!str_cmp(arg1, "cost")) {
		if (arg2[0] == '\0') {
			send_to_char("Get identify cost for what?\n\r", ch);
			return;
		}
		if ((obj = get_obj_here(ch, arg2)) == NULL) {
			act("$n tells you 'You don't have that item'.", mob, NULL, ch, TO_VICT);
			return;
		}

		if (!can_see_obj(mob, obj)) {
			act("$n doesn't see your item.", mob, NULL, ch, TO_VICT);
			return;
		}

		cost = 0;
		if (obj->level > 0) {
			cost = obj->level * 10;
		}

		if (cost == 0) {
			act("$n tells you 'Alright, I will do it for free, for this time.'", mob, NULL, ch, TO_VICT);
		}
		else {
			sprintf(buf,
				"$n tells you 'This will cost to you %d silver and %d gold coins'.",
				cost - (cost / 100) * 100, cost / 100);
			act(buf, mob, obj, ch, TO_VICT);
		}
		return;
	}

	// get item
	if ((obj = get_obj_here(ch, arg1)) == NULL) {
		act("$n tells you 'You don't have that item'.", mob, NULL, ch, TO_VICT);
		return;
	}

	if (!can_see_obj(mob, obj)) {
		act("$n doesn't see your item.", mob, NULL, ch, TO_VICT);
		return;
	}

	cost = 0;
	if (obj->level > 0) {
		cost = obj->level * 10;
	}

	if (cost > (ch->gold * 100 + ch->silver)) {
		act("$n tells you 'You do not have enough gold for my services.'", mob, NULL, ch, TO_VICT);
		return;
	}

	if (cost > 0) {
		sprintf(buf,
			"$n identifies item for %d silver and %d gold coins.",
			cost - (cost / 100) * 100, cost / 100);
		act(buf, mob, obj, ch, TO_VICT);
	}
	else {
		act("$n identifies item free of charge.", mob, NULL, ch, TO_VICT);
	}
	send_to_char("\r\n", ch);
	spell_identify(0, 0, ch, obj, 0);

	deduct_cost(ch, cost);
	mob->gold += cost / 100;
	mob->silver += cost % 100;

}
