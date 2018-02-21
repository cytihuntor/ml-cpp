/*
 * Copyright Elasticsearch B.V. and/or licensed to Elasticsearch B.V. under one
 * or more contributor license agreements. Licensed under the Elastic License;
 * you may not use this file except in compliance with the Elastic License.
 */
#include "CDetectionRulesJsonParserTest.h"

#include <core/CLogger.h>
#include <core/CPatternSet.h>

#include <api/CDetectionRulesJsonParser.h>

#include <boost/unordered_map.hpp>

using namespace ml;
using namespace api;

namespace
{
typedef CDetectionRulesJsonParser::TStrPatternSetUMap TStrPatternSetUMap;
TStrPatternSetUMap EMPTY_VALUE_FILTER_MAP;
}

CppUnit::Test *CDetectionRulesJsonParserTest::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("CDetectionRulesJsonParserTest");

    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenEmptyString",
           &CDetectionRulesJsonParserTest::testParseRulesGivenEmptyString));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenEmptyArray",
           &CDetectionRulesJsonParserTest::testParseRulesGivenEmptyArray));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenArrayContainsStrings",
           &CDetectionRulesJsonParserTest::testParseRulesGivenArrayContainsStrings));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenMissingRuleAction",
           &CDetectionRulesJsonParserTest::testParseRulesGivenMissingRuleAction));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenRuleActionIsNotArray",
           &CDetectionRulesJsonParserTest::testParseRulesGivenRuleActionIsNotArray));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenInvalidRuleAction",
           &CDetectionRulesJsonParserTest::testParseRulesGivenInvalidRuleAction));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenMissingConditionsConnective",
           &CDetectionRulesJsonParserTest::testParseRulesGivenMissingConditionsConnective));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenInvalidConditionsConnective",
           &CDetectionRulesJsonParserTest::testParseRulesGivenInvalidConditionsConnective));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenMissingRuleConditions",
           &CDetectionRulesJsonParserTest::testParseRulesGivenMissingRuleConditions));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenRuleConditionsIsNotArray",
           &CDetectionRulesJsonParserTest::testParseRulesGivenRuleConditionsIsNotArray));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenMissingConditionOperator",
           &CDetectionRulesJsonParserTest::testParseRulesGivenMissingConditionOperator));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenInvalidConditionOperator",
           &CDetectionRulesJsonParserTest::testParseRulesGivenInvalidConditionOperator));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenNumericalActualRuleWithConnectiveOr",
           &CDetectionRulesJsonParserTest::testParseRulesGivenNumericalActualRuleWithConnectiveOr));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenNumericalTypicalAndDiffAbsRuleWithConnectiveAnd",
           &CDetectionRulesJsonParserTest::testParseRulesGivenNumericalTypicalAndDiffAbsRuleWithConnectiveAnd));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenMultipleRules",
           &CDetectionRulesJsonParserTest::testParseRulesGivenMultipleRules));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenCategoricalRule",
           &CDetectionRulesJsonParserTest::testParseRulesGivenCategoricalRule));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenTimeRule",
           &CDetectionRulesJsonParserTest::testParseRulesGivenTimeRule));
    suiteOfTests->addTest(new CppUnit::TestCaller<CDetectionRulesJsonParserTest>(
           "CDetectionRulesJsonParserTest::testParseRulesGivenDifferentActions",
           &CDetectionRulesJsonParserTest::testParseRulesGivenDifferentActions));
    return suiteOfTests;
}

void CDetectionRulesJsonParserTest::testParseRulesGivenEmptyString(void)
{
    LOG_DEBUG("*** testParseRulesGivenEmptyString ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);

    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenEmptyArray(void)
{
    LOG_DEBUG("*** testParseRulesGivenEmptyArray ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules));

    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenArrayContainsStrings(void)
{
    LOG_DEBUG("*** testParseRulesGivenArrayContainsStrings ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[\"a\", \"b\"]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);

    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenMissingRuleAction(void)
{
    LOG_DEBUG("*** testParseRulesGivenMissingRuleAction ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"conditions_connective\":\"or\",";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"condition+type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"5\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);
    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenRuleActionIsNotArray(void)
{
    LOG_DEBUG("*** testParseRulesGivenRuleActionIsNotArray ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"actions\":\"not_array\",";
    rulesJson += "  \"conditions_connective\":\"or\",";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"5\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);
    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenInvalidRuleAction(void)
{
    LOG_DEBUG("*** testParseRulesGivenInvalidRuleAction ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"actions\":[\"something_invalid\"],";
    rulesJson += "  \"conditions_connective\":\"or\",";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"5\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);
    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenMissingConditionsConnective(void)
{
    LOG_DEBUG("*** testParseRulesGivenMissingConditionsConnective ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"5\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);
    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenInvalidConditionsConnective(void)
{
    LOG_DEBUG("*** testParseRulesGivenInvalidConditionsConnective ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"conditions_connective\":\"XOR\",";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"5\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);
    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenMissingRuleConditions(void)
{
    LOG_DEBUG("*** testParseRulesGivenMissingRuleConditions ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"conditions_connective\":\"or\",";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);
    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenRuleConditionsIsNotArray(void)
{
    LOG_DEBUG("*** testParseRulesGivenRuleConditionsIsNotArray ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"conditions_connective\":\"or\",";
    rulesJson += "  \"conditions\": {}";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);
    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenMissingConditionOperator(void)
{
    LOG_DEBUG("*** testParseRulesGivenMissingConditionOperator ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"value\":\"5\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);
    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenInvalidConditionOperator(void)
{
    LOG_DEBUG("*** testParseRulesGivenInvalidConditionOperator ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"ha\",\"value\":\"5\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules) == false);
    CPPUNIT_ASSERT(rules.empty());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenNumericalActualRuleWithConnectiveOr(void)
{
    LOG_DEBUG("*** testParseRulesGivenNumericalActualRuleWithConnectiveOr ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"actions\":[\"filter_results\"],";
    rulesJson += "  \"conditions_connective\":\"or\",";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"5\"}},";
    rulesJson += "    {\"type\":\"numerical_actual\", \"field_name\":\"metric\", \"condition\":{\"operator\":\"lte\",\"value\":\"2.3\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules));

    CPPUNIT_ASSERT_EQUAL(std::size_t(1), rules.size());
    CPPUNIT_ASSERT_EQUAL(std::string("FILTER_RESULTS IF ACTUAL < 5.000000 OR ACTUAL(metric) <= 2.300000"), rules[0].print());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenNumericalTypicalAndDiffAbsRuleWithConnectiveAnd(void)
{
    LOG_DEBUG("*** testParseRulesGivenNumericalTypicalAndDiffAbsRuleWithConnectiveAnd ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"actions\":[\"filter_results\"],";
    rulesJson += "  \"conditions_connective\":\"and\",";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"numerical_typical\", \"condition\":{\"operator\":\"gt\",\"value\":\"5\"}},";
    rulesJson += "    {\"type\":\"numerical_diff_abs\", \"field_name\":\"metric\", \"field_value\":\"cpu\",\"condition\":{\"operator\":\"gte\",\"value\":\"2.3\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules));

    CPPUNIT_ASSERT_EQUAL(std::size_t(1), rules.size());
    CPPUNIT_ASSERT_EQUAL(std::string("FILTER_RESULTS IF TYPICAL > 5.000000 AND DIFF_ABS(metric:cpu) >= 2.300000"), rules[0].print());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenMultipleRules(void)
{
    LOG_DEBUG("*** testParseRulesGivenMultipleRules ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"actions\":[\"filter_results\"],";
    rulesJson += "  \"conditions_connective\":\"or\",";
    rulesJson += "  \"target_field_name\":\"id\",";
    rulesJson += "  \"target_field_value\":\"foo\",";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"1\"}}";
    rulesJson += "  ]";
    rulesJson += "},";
    rulesJson += "{";
    rulesJson += "  \"actions\":[\"skip_sampling\"],";
    rulesJson += "  \"conditions_connective\":\"and\",";
    rulesJson += "  \"target_field_name\":\"id\",";
    rulesJson += "  \"target_field_value\":\"42\",";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"2\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules));

    CPPUNIT_ASSERT_EQUAL(std::size_t(2), rules.size());
    CPPUNIT_ASSERT_EQUAL(std::string("FILTER_RESULTS (id:foo) IF ACTUAL < 1.000000"), rules[0].print());
    CPPUNIT_ASSERT_EQUAL(std::string("SKIP_SAMPLING (id:42) IF ACTUAL < 2.000000"), rules[1].print());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenCategoricalRule(void)
{
    LOG_DEBUG("*** testParseRulesGivenCategoricalRule ***");

    TStrPatternSetUMap filtersById;
    core::CPatternSet filter;
    filter.initFromJson("[\"b\", \"a\"]");
    filtersById["filter1"] = filter;

    CDetectionRulesJsonParser parser(filtersById);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"actions\":[\"filter_results\"],";
    rulesJson += "  \"conditions_connective\":\"or\",";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"categorical\", \"field_name\":\"foo\", \"filter_id\":\"filter1\"}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules));

    CPPUNIT_ASSERT_EQUAL(std::size_t(1), rules.size());
    CPPUNIT_ASSERT_EQUAL(std::string("FILTER_RESULTS IF (foo) IN FILTER"), rules[0].print());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenTimeRule(void)
{
    LOG_DEBUG("*** testParseRulesGivenTimeRule ***");

    CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
    CDetectionRulesJsonParser::TDetectionRuleVec rules;
    std::string rulesJson = "[";
    rulesJson += "{";
    rulesJson += "  \"actions\":[\"filter_results\"],";
    rulesJson += "  \"conditions_connective\":\"and\",";
    rulesJson += "  \"conditions\": [";
    rulesJson += "    {\"type\":\"time\", \"condition\":{\"operator\":\"gte\",\"value\":\"5000\"}}";
    rulesJson += "    ,{\"type\":\"time\", \"condition\":{\"operator\":\"lt\",\"value\":\"10000\"}}";
    rulesJson += "  ]";
    rulesJson += "}";
    rulesJson += "]";

    CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules));
    CPPUNIT_ASSERT_EQUAL(std::size_t(1), rules.size());
    CPPUNIT_ASSERT_EQUAL(std::string("FILTER_RESULTS IF TIME >= 5000.000000 AND TIME < 10000.000000"), rules[0].print());
}

void CDetectionRulesJsonParserTest::testParseRulesGivenDifferentActions(void)
{
    LOG_DEBUG("*** testParseRulesGivenDifferentActions ***");

    {
        CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
        CDetectionRulesJsonParser::TDetectionRuleVec rules;
        std::string rulesJson = "[";
        rulesJson += "{";
        rulesJson += "  \"actions\":[\"filter_results\"],";
        rulesJson += "  \"conditions_connective\":\"and\",";
        rulesJson += "  \"conditions\": [";
        rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"5\"}}";
        rulesJson += "  ]";
        rulesJson += "}";
        rulesJson += "]";

        CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules));

        CPPUNIT_ASSERT_EQUAL(std::size_t(1), rules.size());
        CPPUNIT_ASSERT_EQUAL(std::string("FILTER_RESULTS IF ACTUAL < 5.000000"), rules[0].print());
    }

    {
        CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
        CDetectionRulesJsonParser::TDetectionRuleVec rules;
        std::string rulesJson = "[";
        rulesJson += "{";
        rulesJson += "  \"actions\":[\"skip_sampling\"],";
        rulesJson += "  \"conditions_connective\":\"and\",";
        rulesJson += "  \"conditions\": [";
        rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"5\"}}";
        rulesJson += "  ]";
        rulesJson += "}";
        rulesJson += "]";

        CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules));

        CPPUNIT_ASSERT_EQUAL(std::size_t(1), rules.size());
        CPPUNIT_ASSERT_EQUAL(std::string("SKIP_SAMPLING IF ACTUAL < 5.000000"), rules[0].print());
    }

    {
        CDetectionRulesJsonParser parser(EMPTY_VALUE_FILTER_MAP);
        CDetectionRulesJsonParser::TDetectionRuleVec rules;
        std::string rulesJson = "[";
        rulesJson += "{";
        rulesJson += "  \"actions\":[\"skip_sampling\", \"filter_results\"],";
        rulesJson += "  \"conditions_connective\":\"and\",";
        rulesJson += "  \"conditions\": [";
        rulesJson += "    {\"type\":\"numerical_actual\", \"condition\":{\"operator\":\"lt\",\"value\":\"5\"}}";
        rulesJson += "  ]";
        rulesJson += "}";
        rulesJson += "]";

        CPPUNIT_ASSERT(parser.parseRules(rulesJson, rules));

        CPPUNIT_ASSERT_EQUAL(std::size_t(1), rules.size());
        CPPUNIT_ASSERT_EQUAL(std::string("FILTER_RESULTS AND SKIP_SAMPLING IF ACTUAL < 5.000000"), rules[0].print());
    }
}