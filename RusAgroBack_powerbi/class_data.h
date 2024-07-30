#pragma once
//Класс реализованный для хранения таблицы Data из PostgreSQL
struct object_db
{
    long long int id;
    std::optional<std::string> culture;
    std::optional<std::string> nzp_zp;
    std::optional<std::string> business_dir;
    std::optional<std::tm > calendar_day;
    std::optional<std::string> higher_tm;
    std::optional<std::string> material_order;
    std::optional<float> planned_volume;
    std::optional<float> actual_volume;
    std::optional<std::string> pu;
};

class data
{
public:
    int row_count;
    // Поля класса (атрибуты таблицы data в PostgreSQL)
    std::vector<object_db> objects;

    data(soci::rowset<soci::row> data)
    {
        object_db temp;
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            const soci::row& r = *it;
            temp.id = r.get<long long int>(0);
            temp.culture = r.get_indicator(1) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(1);
            temp.nzp_zp = r.get_indicator(2) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(2);
            temp.business_dir = r.get_indicator(3) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(3);
            temp.calendar_day = r.get_indicator(4) == soci::i_null ? std::optional<std::tm>{} : r.get<std::tm>(4);
            temp.higher_tm = r.get_indicator(5) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(5);
            temp.material_order = r.get_indicator(6) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(6);
            temp.planned_volume = r.get_indicator(7) == soci::i_null ? std::optional<double>{} : r.get<double>(7);
            temp.actual_volume = r.get_indicator(8) == soci::i_null ? std::optional<double>{} : r.get<double>(8);
            temp.pu = r.get_indicator(9) == soci::i_null ? std::optional<std::string>{} : r.get<std::string>(9);
            objects.push_back(temp);
        }

        this->row_count = this->objects.size();
    }

    data()
    {
        this->row_count = 0;
    }

    // Функция для получения уникальных значений material_order
    std::vector<std::string> get_unique_material_orders() const
    {
        std::set<std::string> unique_set;
        for (const auto& obj : objects)
        {
            if (obj.material_order.has_value())
            {
                unique_set.insert(obj.material_order.value());
            }
        }

        return std::vector<std::string>(unique_set.begin(), unique_set.end());
    }

    void print()
    {
        std::cout << this->row_count << std::endl;
        for (size_t i = 0; i < this->row_count; i++)
        {
            std::cout << objects[i].id << std::endl;
            std::cout << objects[i].material_order.value() << std::endl;
        }
    }
};

std::string to_string(const char8_t* str) 
{
    return std::string(reinterpret_cast<const char*>(str));
}


// Cчитывание таблицы по каждой культуре в массив в PostgreSQL
void read_table_data(soci::session& sql, data data_shbn[CULTURES_COUNT])
{
    for (int i = 0; i < CULTURES_COUNT; i++)
    {
            std::string culture_str = to_string(CULTURES_RUS[i]);
            soci::rowset<soci::row> rs = (sql.prepare << "SELECT * FROM platform_shbn_data WHERE culture = '" << culture_str << "'");
            data_shbn[i] = data(rs);
    }
}