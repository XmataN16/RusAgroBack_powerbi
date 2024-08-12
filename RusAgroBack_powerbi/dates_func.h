#pragma once
#include "cmath"
//������� �������� ������� ����
std::tm create_date(int year, int month, int day) 
{
    std::tm date;
    std::memset(&date, 0, sizeof(date)); // ������� ��������� (�������� ����)

    date.tm_year = year - 1900; // ��� � 1900
    date.tm_mon = month - 1;    // ����� (������ = 0, ������� = 1, ..., ������� = 11)
    date.tm_mday = day;         // ���� ������

    return date;
}

//������� ��������� ���� ���
std::optional<std::tm> max_date(const std::tm& date1, const std::tm& date2)
{
    std::time_t time1 = mktime(const_cast<std::tm*>(&date1));
    std::time_t time2 = mktime(const_cast<std::tm*>(&date2));

    if (difftime(time1, time2) > 0)
    {
        return date1;
    }
    else
    {
        return date2;
    }
}

//������� ��������� ���� ���
std::optional<std::tm> min_date(const std::optional<std::tm>& date1, const std::optional<std::tm>& date2)
{
    if (!date1.has_value() and !date2.has_value())
    {
        return std::nullopt;
    }
    else if (date1.has_value() and !date2.has_value())
    {
        return date1;
    }
    else if (!date1.has_value() and date2.has_value())
    {
        return date2;
    }
    else
    {
        std::time_t time1 = mktime(const_cast<std::tm*>(&date1.value()));
        std::time_t time2 = mktime(const_cast<std::tm*>(&date2.value()));
        if (difftime(time1, time2) > 0)
        {
            return date2;
        }
        else
        {
            return date1;
        }
    }
}

bool max_date_bool(const std::tm& date1, const std::tm& date2)
{
    std::time_t time1 = std::mktime(const_cast<std::tm*>(&date1));
    std::time_t time2 = std::mktime(const_cast<std::tm*>(&date2));

    return std::difftime(time1, time2) > 0;
}

// �������� "������" ��� ���� std::optional<std::tm>
bool operator > (const std::optional<std::tm>& tm1, const std::optional<std::tm>& tm2)
{
    std::tm tm1_value = tm1.value();
    std::tm tm2_value = tm2.value();

    // ����������� std::tm � time_t ��� ���������
    std::time_t time1 = std::mktime(&tm1_value);
    std::time_t time2 = std::mktime(&tm2_value);

    if (std::difftime(time1, time2) > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// �������� "������" ��� ���� std::optional<std::tm>
bool operator < (const std::optional<std::tm>& tm1, const std::optional<std::tm>& tm2)
{
    std::tm tm1_value = tm1.value();
    std::tm tm2_value = tm2.value();

    // ����������� std::tm � time_t ��� ���������
    std::time_t time1 = std::mktime(&tm1_value);
    std::time_t time2 = std::mktime(&tm2_value);

    if (std::difftime(time1, time2) < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// �������� "������ ��� �����" ��� ���� std::optional<std::tm>
bool operator <= (const std::optional<std::tm>& tm1, const std::optional<std::tm>& tm2)
{
    std::tm tm1_value = tm1.value();
    std::tm tm2_value = tm2.value();

    // ����������� std::tm � time_t ��� ���������
    std::time_t time1 = std::mktime(&tm1_value);
    std::time_t time2 = std::mktime(&tm2_value);

    if (std::difftime(time1, time2) <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// �������� "������ ��� �����" ��� ���� std::optional<std::tm>
bool operator >= (const std::optional<std::tm>& tm1, const std::optional<std::tm>& tm2)
{
    std::tm tm1_value = tm1.value();
    std::tm tm2_value = tm2.value();

    // ����������� std::tm � time_t ��� ���������
    std::time_t time1 = std::mktime(&tm1_value);
    std::time_t time2 = std::mktime(&tm2_value);

    if (std::difftime(time1, time2) >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// �������� "�����" ��� ���� std::optional<std::tm>
bool operator == (const std::optional<std::tm>& tm1, const std::optional<std::tm>& tm2)
{
    std::tm tm1_value = tm1.value();
    std::tm tm2_value = tm2.value();

    // ����������� std::tm � time_t ��� ���������
    std::time_t time1 = std::mktime(&tm1_value);
    std::time_t time2 = std::mktime(&tm2_value);

    if (std::difftime(time1, time2) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ������� ��� ���������� ������������ ���� ����� ��� ��� � ������ �������� nullopt
std::optional<std::tm> custom_max(const std::vector<std::optional<std::tm>>& dates)
{
    if (!dates[0].has_value() and !dates[1].has_value() and !dates[2].has_value())
    {
        return std::nullopt;
    }
    else if (dates[0].has_value() and dates[1].has_value() and dates[2].has_value())
    {
        if (dates[0] > dates[1])
        {
            if (dates[0] > dates[2])
            {
                return dates[0];
            }
            else
            {
                return dates[2];
            }
        }
        else
        {
            if (dates[1] > dates[2])
            {
                return dates[1];
            }
            else
            {
                return dates[2];
            }
        }
    }
    else if (dates[0].has_value() and dates[1].has_value() and !dates[2].has_value())
    {
        if (dates[0] > dates[1])
        {
            return dates[0];
        }
        else
        {
            return dates[1];
        }
    }
    else if (dates[0].has_value() and !dates[1].has_value() and dates[2].has_value())
    {
        if (dates[0] > dates[2])
        {
            return dates[0];
        }
        else
        {
            return dates[2];
        }
    }
    else if (!dates[0].has_value() and dates[1].has_value() and dates[2].has_value())
    {
        if (dates[1] > dates[2])
        {
            return dates[1];
        }
        else
        {
            return dates[2];
        }
    }
    else if (!dates[0].has_value() and !dates[1].has_value() and dates[2].has_value())
    {
        return dates[2];
    }
    else if (!dates[0].has_value() and dates[1].has_value() and !dates[2].has_value())
    {
        return dates[1];
    }
    else if (dates[0].has_value() and !dates[1].has_value() and !dates[2].has_value())
    {
        return dates[0];
    }
}

// ������� ��� ���������� ����������� ���� ����� ��� ��� � ������ �������� nullopt
std::optional<std::tm> custom_min(const std::vector<std::optional<std::tm>>& dates)
{
    if (!dates[0].has_value() and !dates[1].has_value() and !dates[2].has_value())
    {
        return std::nullopt;
    }
    else if (dates[0].has_value() and dates[1].has_value() and dates[2].has_value())
    {
        if (dates[0] < dates[1])
        {
            if (dates[0] < dates[2])
            {
                return dates[0];
            }
            else
            {
                return dates[2];
            }
        }
        else
        {
            if (dates[1] < dates[2])
            {
                return dates[1];
            }
            else
            {
                return dates[2];
            }
        }
    }
    else if (dates[0].has_value() and dates[1].has_value() and !dates[2].has_value())
    {
        if (dates[0] < dates[1])
        {
            return dates[0];
        }
        else
        {
            return dates[1];
        }
    }
    else if (dates[0].has_value() and !dates[1].has_value() and dates[2].has_value())
    {
        if (dates[0] < dates[2])
        {
            return dates[0];
        }
        else
        {
            return dates[2];
        }
    }
    else if (!dates[0].has_value() and dates[1].has_value() and dates[2].has_value())
    {
        if (dates[1] < dates[2])
        {
            return dates[1];
        }
        else
        {
            return dates[2];
        }
    }
    else if (!dates[0].has_value() and !dates[1].has_value() and dates[2].has_value())
    {
        return dates[2];
    }
    else if (!dates[0].has_value() and dates[1].has_value() and !dates[2].has_value())
    {
        return dates[1];
    }
    else if (dates[0].has_value() and !dates[1].has_value() and !dates[2].has_value())
    {
        return dates[0];
    }
}

// �������������� ������ � std::tm
std::optional<std::tm> str_to_tm(const std::optional<std::string>& str)
{
    if (str.has_value())
    {
        std::optional<std::tm> tm = {};
        std::istringstream ss(str.value());
        ss >> std::get_time(&tm.value(), "%Y-%m-%d");
        return tm;
    }
    else
    {
        return std::nullopt;
    }
}

// �������������� std::tm ������� � ������
std::optional<std::string> tm_to_str(const std::optional<std::tm>& tm)
{
    if (tm.has_value())
    {
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm.value());
        return std::string(buffer);
    }
    else
    {
        return std::nullopt;
    }
}

// ���������� ���� � ����
std::optional<std::tm> add_days(const std::optional<std::tm>& tm, int days)
{
    std::optional<std::tm> result = tm;
    if (result.has_value())
    {
        time_t time = mktime(&result.value());
        time += days * 24 * 60 * 60; // ���������� ���� � ��������
        result = *localtime(&time);
        return result;
    }
    else
    {
        return std::nullopt;
    }

}