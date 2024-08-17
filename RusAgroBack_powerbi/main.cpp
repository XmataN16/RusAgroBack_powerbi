#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <locale>
#include <optional>
#include <windows.h>
#include <time.h>
#include <iomanip>
#include <ctime>
#include <list>
#include <cstdio>
#include <utility>
#include <fstream>
#include <nlohmann/json.hpp>
#include <omp.h>

//�������� � �������
const int CULTURES_COUNT = 5;
const int REGIONS_COUNT = 7;

const std::string CULTURES[CULTURES_COUNT] = { "sugar_beet", "soy", "sunflower", "corn", "corn_silage" };

const std::string CULTURES_RUS[CULTURES_COUNT] = { u8"�������� ������", u8"���", u8"������������", u8"�������� �� �����", u8"�������� �� �����" };

const std::string REGIONS[REGIONS_COUNT] = { "regionbels", "regionbelc", "regionbelk", "regiontams", "regiontamn", "regionorel", "regionprim", };

const std::string REGIONS_RUS[REGIONS_COUNT] = { u8"�������� ��", u8"�������� �����", u8"�������� - �����", u8"������ - ��", u8"������ - �����", u8"����", u8"��������" };

#include "dates_func.h"
#include "class_data.h"
#include "data_struct.h"
#include "deadlines.h"



int main()
{
    SetConsoleOutputCP(65001);
    try
    {
        int num_threads = omp_get_max_threads(); // ������ ������������ ����� �������
        omp_set_num_threads(num_threads); // ������������� ����� �������

        //����������� � �� PostgreSQL
        soci::session sql(soci::postgresql, "dbname=agro_system user=xmatan16 password=matematic16 hostaddr=127.0.0.1 port=5432");
        //���������� data
        data data_shbn[CULTURES_COUNT];
        LastResult lastResult;
        
        read_table_data(sql, data_shbn);

        clock_t start = clock();

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                calc_sugar_beet(data_shbn, lastResult);
            }
            #pragma omp section
            {
                calc_soy(data_shbn, lastResult);
            }
            #pragma omp section
            {
                calc_sunflower(data_shbn, lastResult);
            }
            #pragma omp section
            {
                calc_corn(data_shbn, lastResult);
            }
            #pragma omp section
            {
                calc_corn_silage(data_shbn, lastResult);
            }
        }

        clock_t end = clock();
        double seconds = (double)(end - start) / CLOCKS_PER_SEC;
        printf("The time: %f seconds\n", seconds);

        writeReportToJson(lastResult, "report.json");
    }
    catch (const soci::soci_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
	
	return 0;
}