#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

TEST(FakeTest, PleaseDeleteOnceYouWriteSome) {
  // If no tests exist, GoogleTest fails to compile with a fairly confusing
  // message. We have this empty test that does nothing to allow an empty
  // autograder submission to compile and regularly fail. Once you write your
  // own tests for the project, you can delete this one.
  EXPECT_EQ(1, 1);
}
TEST(CleanToken, NoPunctuationOrSpecialCharacters) {
  ASSERT_THAT(cleanToken("hello"), StrEq("hello"));
  ASSERT_THAT(cleanToken("world"), StrEq("world"));
}

TEST(CleanToken, PunctuationAtBeginning) {
  ASSERT_THAT(cleanToken("....hello"), StrEq("hello"));
  ASSERT_THAT(cleanToken("!!world"), StrEq("world"));
}

TEST(CleanToken, PunctuationAtEnd) {
  ASSERT_THAT(cleanToken("hello!!"), StrEq("hello"));
  ASSERT_THAT(cleanToken("world...."), StrEq("world"));
}

TEST(CleanToken, NoLetters) {
  ASSERT_THAT(cleanToken("135820"), StrEq(""));
  ASSERT_THAT(cleanToken("!!!..."), StrEq(""));
  ASSERT_THAT(cleanToken("....$$$$......"), StrEq(""));
  ASSERT_THAT(cleanToken("....2312^#@@@...."), StrEq(""));
}

TEST(CleanToken, UppercaseLettersAndPunctuation) {
  ASSERT_THAT(cleanToken("HELLO"), StrEq("hello"));
  ASSERT_THAT(cleanToken("WORLD..."), StrEq("world"));
  ASSERT_THAT(cleanToken("..AGAIN"), StrEq("again"));
}

TEST(CleanToken, PunctuationBothSides) {
  ASSERT_THAT(cleanToken("!!!hello!!"), StrEq("hello"));
  ASSERT_THAT(cleanToken("..!!world...."), StrEq("world"));
}
TEST(CleanToken, PunctuationBothSidesAndMiddle) {
  ASSERT_THAT(cleanToken("!!!hel...lo!!"), StrEq("hel...lo"));
  ASSERT_THAT(cleanToken("hel...lo!!"), StrEq("hel...lo"));
  ASSERT_THAT(cleanToken("..!!wor!!ld"), StrEq("wor!!ld"));
  ASSERT_THAT(cleanToken("wor!!ld"), StrEq("wor!!ld"));
}
TEST(CleanToken, NumbersAtTheEnds) {
  ASSERT_THAT(cleanToken("helloo3455"), StrEq("helloo3455"));
  ASSERT_THAT(cleanToken("12world45"), StrEq("12world45"));
}
TEST(CleanToken, OneLetterToken) {
  ASSERT_THAT(cleanToken(".....!!!a"), StrEq("a"));
  ASSERT_THAT(cleanToken("a.....!!!!!!......."), StrEq("a"));
  ASSERT_THAT(cleanToken(".........a.!!!!....."), StrEq("a"));
}
TEST(CleanToken, NoPunctuationNonLetterNonDigit) {
  ASSERT_THAT(cleanToken("hello"
                         ""),
              StrEq("hello"));
  ASSERT_THAT(cleanToken("world#"), StrEq("world"));
}
TEST(GatherTokens, LeadingSpaces) {
  string result = ("     hello world");
  set<string> expected = {"hello", "world"};
  EXPECT_THAT(gatherTokens(result), ContainerEq(expected))
      << "text=\"" << result << "\"";
}
TEST(GatherTokens, NormalText) {
  string result = ("Writing hello world");
  set<string> expected = {"writing", "hello", "world"};
  EXPECT_THAT(gatherTokens(result), ContainerEq(expected))
      << "text=\"" << result << "\"";
}
TEST(GatherTokens, TrailingSpaces) {
  string result = ("hello world      ");
  set<string> expected = {"hello", "world"};
  EXPECT_THAT(gatherTokens(result), ContainerEq(expected))
      << "text=\"" << result << "\"";
}
TEST(GatherTokens, MultipleSpacesBetweenWords) {
  string result = ("hello         world");
  set<string> expected = {"hello", "world"};
  EXPECT_THAT(gatherTokens(result), ContainerEq(expected))
      << "text=\"" << result << "\"";
}
TEST(GatherTokens, BothSidesSpaces) {
  string result = ("     hello   world    ");
  set<string> expected = {"hello", "world"};
  EXPECT_THAT(gatherTokens(result), ContainerEq(expected))
      << "text=\"" << result << "\"";
}
TEST(GatherTokens, TextWithEndsPunctuation) {
  string result = ("...Hello world! Hello,,");
  set<string> expected = {"hello", "world"};
  EXPECT_THAT(gatherTokens(result), ContainerEq(expected))
      << "text=\"" << result << "\"";
}
TEST(GatherTokens, TextWithMiddlePunctuations) {
  string result = ("Hel..lo wor..ld");
  set<string> expected = {"hel..lo", "wor..ld"};
  EXPECT_THAT(gatherTokens(result), ContainerEq(expected))
      << "text=\"" << result << "\"";
}
TEST(GatherTokens, EmptyString) {
  string result = ("");
  set<string> expected = {};
  EXPECT_THAT(gatherTokens(result), ContainerEq(expected))
      << "text=\"" << result << "\"";
}
TEST(BuildIndex, TinierTxt) {
  string filename = "data/tinier.txt";
  map<string, set<string>> expectedIndex = {
      {"to", {"www.example.com", "www.otherexample.com"}},
      {"be", {"www.example.com", "www.otherexample.com"}},
      {"ora", {"www.example.com"}},
      {"not", {"www.example.com"}},
      {"want", {"www.otherexample.com"}},
      {"free", {"www.otherexample.com"}},
  };
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(2)) << retTestFeedback;
}
TEST(BuildIndex, TinyTxt) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedIndex = {
      {"eggs", {"www.shoppinglist.com"}},
      {"milk", {"www.shoppinglist.com"}},
      {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
      {"bread", {"www.shoppinglist.com"}},
      {"cheese", {"www.shoppinglist.com"}},
      {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"gre-en", {"www.rainbow.org"}},
      {"orange", {"www.rainbow.org"}},
      {"yellow", {"www.rainbow.org"}},
      {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"indigo", {"www.rainbow.org"}},
      {"violet", {"www.rainbow.org"}},
      {"one", {"www.dr.seuss.net"}},
      {"two", {"www.dr.seuss.net"}},
      {"i'm", {"www.bigbadwolf.com"}},
      {"not", {"www.bigbadwolf.com"}},
      {"trying", {"www.bigbadwolf.com"}},
      {"to", {"www.bigbadwolf.com"}},
      {"eat", {"www.bigbadwolf.com"}},
      {"you", {"www.bigbadwolf.com"}},
  };
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(4)) << retTestFeedback;
}

TEST(BuildIndex, DoesNotExist) {
  string filename = "data/notExist.txt";
  map<string, set<string>> expectedIndex = {};
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(0)) << retTestFeedback;
}
map<string, set<string>> INDEX = {
    {"tasty", {"recipes.com", "foodblog.com", "nutrition.org"}},
    {"healthy", {"foodblog.com", "nutrition.org"}},
    {"fruits", {"nutrition.org"}},
    {"mushrooms", {"foodblog.com"}},
    {"cheap", {"budgetmeals.com", "foodblog.com"}},
    {"simple", {"budgetmeals.com"}},
    {"vegan", {"nutrition.org", "veganfood.com"}},
    {"fast", {"foodblog.com", "budgetmeals.com"}},
};

TEST(FindQueryMatches, SingleTerm) {
  set<string> expected;

  expected = {"recipes.com", "foodblog.com", "nutrition.org"};
  EXPECT_THAT(findQueryMatches(INDEX, "tasty"), ContainerEq(expected));

  expected = {"nutrition.org", "veganfood.com"};
  EXPECT_THAT(findQueryMatches(INDEX, "vegan.."), ContainerEq(expected));
  expected = {"nutrition.org"};
  EXPECT_THAT(findQueryMatches(INDEX, "fruits!"), ContainerEq(expected));
}

TEST(FindQueryMatches, QueryOR) {
  set<string> expected;

  expected = {"nutrition.org", "budgetmeals.com"};
  EXPECT_THAT(findQueryMatches(INDEX, "fruits simple"), ContainerEq(expected));

  expected = {"recipes.com", "foodblog.com", "nutrition.org"};
  EXPECT_THAT(findQueryMatches(INDEX, "tasty healthy fruits"),
              ContainerEq(expected));
  EXPECT_THAT(findQueryMatches(INDEX, "TASTY, HEALTHY.., FRUITS!"),
              ContainerEq(expected));
}

TEST(FindQueryMatches, QueryAnd) {
  set<string> expected;

  expected = {"foodblog.com", "budgetmeals.com"};
  EXPECT_THAT(findQueryMatches(INDEX, "cheap +fast"), ContainerEq(expected));

  expected = {"foodblog.com"};
  EXPECT_THAT(findQueryMatches(INDEX, "tasty +fast"), ContainerEq(expected));

  expected = {"nutrition.org"};
  EXPECT_THAT(findQueryMatches(INDEX, "fruits +vegan +healthy"),
              ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "..vegan +mushrooms"),
              ContainerEq(expected));
}

TEST(FindQueryMatches, QueryDifference) {
  set<string> expected;

  expected = {"recipes.com", "foodblog.com"};
  EXPECT_THAT(findQueryMatches(INDEX, "tasty -vegan"), ContainerEq(expected));

  expected = {"foodblog.com"};
  EXPECT_THAT(findQueryMatches(INDEX, "healthy -fruits"),
              ContainerEq(expected));

  expected = {"recipes.com", "nutrition.org"};
  EXPECT_THAT(findQueryMatches(INDEX, "tasty -simple -fast"),
              ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "simple -cheap"), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "..mushrooms -healthy -tasty"),
              ContainerEq(expected));
}

TEST(FindQueryMatches, LongCombinedQueries) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "tasty +fruits simple -fast +cheap"),
              ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "vegan -fruits +mushrooms"),
              ContainerEq(expected));

  expected = {"foodblog.com", "nutrition.org", "veganfood.com"};
  EXPECT_THAT(findQueryMatches(INDEX, "healthy -simple +fast vegan"),
              ContainerEq(expected));
}
TEST(FindQueryMatches, FirstTermNotInIndex) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "WordNotThere"), ContainerEq(expected));

  EXPECT_THAT(findQueryMatches(INDEX, "MissingWord +tasty"),
              ContainerEq(expected));
}

TEST(FindQueryMatches, LaterTermNotInIndexWithModifier) {
  set<string> expected;

  expected = {"foodblog.com", "nutrition.org"};
  EXPECT_THAT(findQueryMatches(INDEX, "tasty +healthy -NoWord"),
              ContainerEq(expected));

  expected = {"budgetmeals.com"};
  EXPECT_THAT(findQueryMatches(INDEX, "simple -NoTerm"), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "tasty +NotHere"), ContainerEq(expected));

  expected = {"nutrition.org"};
  EXPECT_THAT(findQueryMatches(INDEX, "fruits CannotFindIt"),
              ContainerEq(expected));
}

TEST(FindQueryMatches, NoTermsInIndex) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "doesnExit +plusDoesntExist"),
              ContainerEq(expected));

  EXPECT_THAT(findQueryMatches(INDEX, "thisIsNotThere -NotThere"),
              ContainerEq(expected));
}
