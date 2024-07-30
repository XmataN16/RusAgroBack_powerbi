#pragma once

//���������� ������� �������� � ����������, ���������� � ������� 
void set_deadlines(std::map<std::string, std::map<std::string, int>>& deadlines)
{
	//�� �������� ������
	deadlines["�������� ������"]["�������� ���������� ���������"] = 3;
	deadlines["�������� ������"]["������ ����������� ���������"] = 18;
	deadlines["�������� ������"]["������ ����������� ���������"] = 10;
	deadlines["�������� ������"]["������ ����������� ���������"] = 10;
	deadlines["�������� ������"]["��������� ����������� ���������"] = 10;
	//�� ���
	deadlines["���"]["�������� ���������� ���������"] = 3;
	deadlines["���"]["������ ��������� ����������� ������ �����������������"] = 20;
	deadlines["���"]["������ ��������� ����������� ������ �����������������"] = 12;
	//�� ��������
	deadlines["��������"]["�������� ���������� ���������"] = 3;
	deadlines["��������"]["��������� ����������� ������ ������������������+ ����������"] = 30;
	//�� �������������
	deadlines["������������"]["��������� ����������� ������ �������������������"] = 25;
	deadlines["������������"]["��������� ����������� ������ �������������"] = 7;
}

struct Status 
{
    int fieldsCount;
    int fieldsSquare;
};

struct Operation 
{
    std::string businessDirection;
    std::string PU;
    std::unordered_map<std::string, Status> statuses;
};

struct Crop 
{
    std::unordered_map<std::string, std::vector<Operation>> operations;
};

struct Result 
{
	std::unordered_map<std::string, std::vector<std::string>> sugar_beet;
};

struct LastResult 
{
    Crop sugar_beet;
    Crop corn;
    Crop soy;
    Crop sunflower;
    Crop corn_silage;
};

/*
LastResult lastResult = {
	.sugar_beet = {
		.operations = {
			"������ �����������(��)" = {}
        }
    },
};
*/

// ������� ��� ��������� ���������� �������� material_order
std::vector<std::string> get_unique_material_orders(const std::vector<object_db>& data_in_culture) 
{
	std::set<std::string> unique_set;
	for (const auto& obj : data_in_culture)
	{
		if (obj.material_order.has_value()) 
		{
			unique_set.insert(obj.material_order.value());
		}
	}
	return std::vector<std::string>(unique_set.begin(), unique_set.end());
}

void calc(data data_shbn[CULTURES_COUNT])
{
	Result lastResult;
	std::vector<object_db> data_sugar_beet = data_shbn[0].objects;
	std::vector<std::string> uniq_operations_sugar_beet = get_unique_material_orders(data_sugar_beet);

	for (size_t i = 0; i < uniq_operations_sugar_beet.size(); i++)
	{
		lastResult.sugar_beet[uniq_operations_sugar_beet[i]].push_back({ uniq_operations_sugar_beet[i]});
		std::cout << lastResult.sugar_beet[uniq_operations_sugar_beet[i]][i] << std::endl;
	}
	
    //std::cout << data_sugar_beet[0].material_order.value() << std::endl;
	
	
}

// result
/*{sugar_beet:
  {not_started:
	[
	  {ID: 54028
		Culture: �������� ������
		NZP_ZP: ZP
		Business Direction: �������� ��
		Calendar Day: 2024-08-25
		Higher TM: BL-04-04-83-0050
		Material Order: ������ ����������� (��)
		Planned Volume: 49
		Actual Volume: 0
		PU: ��� �� ������������
	  }
	]
  }
}
*/