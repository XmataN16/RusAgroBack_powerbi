#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>
#include <iostream>
#include <vector>
#include <map>
#include <optional>
#include <windows.h>
#include <time.h>
#include <iomanip>
#include <ctime>
#include <nlohmann/json.hpp>
#include "deadlines.h"

//Культуры и регионы
const int CULTURES_COUNT = 5;
const int REGIONS_COUNT = 7;

const std::string CULTURES[CULTURES_COUNT] = { "sugar_beet", "soy", "sunflower", "corn", "corn_silage" };

const std::string CULTURES_RUS[CULTURES_COUNT] = { u8"Сахарная свекла", u8"Соя", u8"Подсолнечник", u8"Кукуруза на зерно", u8"Кукуруза на силос" };

const std::string REGIONS[REGIONS_COUNT] = { "regionbels", "regionbelc", "regionbelk", "regiontams", "regiontamn", "regionorel", "regionprim", };

const std::string REGIONS_RUS[REGIONS_COUNT] = { u8"Белгород Юг",  u8"Белгород Центр", u8"Белгород-Курск", u8"Тамбов-Юг", u8"Тамбов-Север", u8"Орел", u8"Приморье" };

#include "deadlines.h"
#include "class_data.h"

int main()
{
    SetConsoleOutputCP(65001);
    try
    {
        //подключение к БД PostgreSQL
        soci::session sql(soci::postgresql, "dbname=agro_system user=xmatan16 password=matematic16 hostaddr=127.0.0.1 port=5432");

        //создаем и считываем словарь сроков
        std::map<std::string, std::map<std::string, int>> deadlines;
        set_deadlines(deadlines);
        //считывание data
        data data_shbn[CULTURES_COUNT][REGIONS_COUNT];
        read_table_data(sql, data_shbn);

        data_shbn[0][0].print();
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
	
	return 0;
}