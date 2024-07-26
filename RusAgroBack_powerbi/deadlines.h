#pragma once

class ResultArray
{
public:


	ResultArray()
	{

	}


private:

};



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

struct Result 
{
	std::unordered_map<std::string, std::vector<std::string>> sugar_beet;
	std::unordered_map<std::string, std::vector<std::string>> corn;
};

struct CultureResult 
{
	std::vector<std::string> non_feed;
	std::vector<std::string> not_started;
};

void calc(data data_shbn[CULTURES_COUNT][REGIONS_COUNT])
{
	Result result;
	CultureResult sugar_beet_result;
	CultureResult corn;

	for (int culture = 0; culture < CULTURES_COUNT; culture++)
	{
		for (int region = 0; region < REGIONS_COUNT; region++)
		{
			for (int row = 0; row < data_shbn[culture][region].row_count; row++)
			{
				// ������ �� �� �� ����, ��������� ����������� � ����...
				
			}
		}
	}
	for (const auto& el : data_shbn) 
	{
		if (el.culture == "sugar_beet") 
		{
			// Logic for calculating status
			// Assign status to el.status
			// End of status calculation logic
			if (el.status == "�� �������") sugar_beet_result.non_feed.push_back(cur_el);
			if (el.status == "���� �� ��������") sugar_beet_result.not_started.push_back(cur_el);
		}
	}

	result.sugar_beet = sugar_beet_result;

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