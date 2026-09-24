#include <unity.h>

#include "Votes.h"

void test_moods_use_one_based_public_keys() {
    String candidates[] = {String("Equipe 0"), String("Equipe 1")};
    Votes votes;
    votes.init(candidates, 2);

    votes.getItemName(0)->incrementVotes(1);
    votes.getItemName(0)->incrementVotes(2);
    votes.getItemName(0)->incrementVotes(3);

    TEST_ASSERT_EQUAL_UINT16(1, votes.getItemName(0)->getMoods(1));
    TEST_ASSERT_EQUAL_UINT16(1, votes.getItemName(0)->getMoods(2));
    TEST_ASSERT_EQUAL_UINT16(1, votes.getItemName(0)->getMoods(3));
    TEST_ASSERT_EQUAL_UINT16(0, votes.getItemName(0)->getMoods(4));
}

void test_invalid_candidate_access_is_safe() {
    String candidates[] = {String("Equipe 0")};
    Votes votes;
    votes.init(candidates, 1);

    TEST_ASSERT_NULL(votes.getItemName(-1));
    TEST_ASSERT_NULL(votes.getItemName(1));
}

void test_serialization_round_trip() {
    String candidates[] = {String("Equipe 0"), String("Equipe 1")};
    Votes original;
    original.init(candidates, 2);
    original.getItemName(0)->incrementVotes(1);
    original.getItemName(0)->incrementVotes(1);
    original.getItemName(1)->incrementVotes(3);

    Votes restored;
    restored.init(candidates, 2);
    restored.load_from_String(original.to_string());

    TEST_ASSERT_EQUAL_UINT16(2, restored.getItemName(0)->getMoods(1));
    TEST_ASSERT_EQUAL_UINT16(1, restored.getItemName(1)->getMoods(3));
}

void test_invalid_serialized_values_reset_item() {
    String candidates[] = {String("Equipe 0")};
    Votes votes;
    votes.init(candidates, 1);
    votes.getItemName(0)->incrementVotes(1);
    votes.load_from_String(String("-1;2;3;|"));

    TEST_ASSERT_EQUAL_UINT16(0, votes.getItemName(0)->getMoods(1));
    TEST_ASSERT_EQUAL_UINT16(0, votes.getItemName(0)->getMoods(2));
    TEST_ASSERT_EQUAL_UINT16(0, votes.getItemName(0)->getMoods(3));
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_moods_use_one_based_public_keys);
    RUN_TEST(test_invalid_candidate_access_is_safe);
    RUN_TEST(test_serialization_round_trip);
    RUN_TEST(test_invalid_serialized_values_reset_item);
    UNITY_END();
}

void loop() {}

int main() {
    setup();
    return 0;
}
