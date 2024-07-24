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

//�������� � �������
const int CULTURES_COUNT = 5;
const int REGIONS_COUNT = 7;

const std::string CULTURES[CULTURES_COUNT] = { "sugar_beet", "soy", "sunflower", "corn", "corn_silage" };

const std::string CULTURES_RUS[CULTURES_COUNT] = { u8"�������� ������", u8"���", u8"������������", u8"�������� �� �����", u8"�������� �� �����" };

const std::string REGIONS[REGIONS_COUNT] = { "regionbels", "regionbelc", "regionbelk", "regiontams", "regiontamn", "regionorel", "regionprim", };

const std::string REGIONS_RUS[REGIONS_COUNT] = { u8"�������� ��",  u8"�������� �����", u8"��������-�����", u8"������-��", u8"������-�����", u8"����", u8"��������" };

#include "deadlines.h"
#include "class_data.h"

int main()
{
    SetConsoleOutputCP(65001);
    try
    {
        //����������� � �� PostgreSQL
        soci::session sql(soci::postgresql, "dbname=agro_system user=xmatan16 password=matematic16 hostaddr=127.0.0.1 port=5432");

        //������� � ��������� ������� ������
        std::map<std::string, std::map<std::string, int>> deadlines;
        set_deadlines(deadlines);
        //���������� data
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