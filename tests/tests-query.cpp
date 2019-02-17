#include <catch2/catch.hpp>

#include <iostream>

#include <shixxy/Database.hpp>
#include <shixxy/Query.hpp>

TEST_CASE("Query can be used to create a table", "[Query]") {
    Sxy::Database db;
    db.connect(":memory:");

    Sxy::Query query {db};
    bool res_prep = query.prepare("CREATE TABLE cats ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT);");
    REQUIRE(res_prep == true);

    bool res_exec = query.exec();
    REQUIRE(res_exec == true);

    SECTION("Query can be used to INSERT") {
        Sxy::Query query {db};
        auto res_prep = query.prepare(
                "INSERT INTO cats (name) VALUES ('Chloe');");
        auto res_exec = query.exec();

        REQUIRE(res_prep == true);
        REQUIRE(res_exec == true);
    }
}

TEST_CASE("Query methods return false with bad statements", "[Query]") {
    Sxy::Database db;
    db.connect(":memory:");

    Sxy::Query query {db};
    bool res_prep = query.prepare("bad query syntax");
    REQUIRE(res_prep == false);

    /*
    std::cout << "LAST QUERY: " << query.last_query() << '\n';
    std::cout << "LAST ERROR: " << query.last_error() << '\n';
    */
}

TEST_CASE("Query report returns accurate information", "[Query::last_*]") {
    Sxy::Database db;
    db.connect(":memory:");

    Sxy::Query query1 {db};
    query1.prepare("INSERT INTO missing_table (name) VALUES ('Error');");
    query1.exec();

    REQUIRE(query1.last_error() == "no such table: missing_table");

    Sxy::Query query2 {db};
    query2.prepare("CREATE TABLE dogs (name TEXT);");
    query2.exec();

    REQUIRE(query2.last_query() == "CREATE TABLE dogs (name TEXT);");
}

/*
TEST_CASE("Query::step can be used to retrieve sets", "[Query::step]") {
    Sxy::Database db;
    db.connect(":memory:");

    Sxy::Query query {db};
    bool res = query.prepare("CREATE TABLE cats ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT);");
    REQUIRE(res == true);
    res = query.exec();
    REQUIRE(res == true);

    query = Sxy::Query{db};
    query.prepare("INSERT INTO cats (name) "
            "VALUES ('Chloe');");
    res = query.exec();
    REQUIRE(res == true);

    query = Sxy::Query{db};
    query.prepare("INSERT INTO cats (name) "
            "VALUES ('Magnus');");
    res = query.exec();
    REQUIRE(res == true);

    std::vector<std::string> vnames;
}
*/