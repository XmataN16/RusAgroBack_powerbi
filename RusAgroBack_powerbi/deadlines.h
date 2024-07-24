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

void calc(data data_shbn[CULTURES_COUNT][REGIONS_COUNT])
{
	// ������ �� ���� ���������
	for (int culture = 0; culture < CULTURES_COUNT; culture++) 
	{
		// �������� ������� ��� ������� ��������
		std::vector<int> cultureResult;
		// ��� �������� �� ��������
		std::unordered_map<std::string, std::string> cultureOperations;

		// ���������� ������� ���������� �� ������� ��������
		for (int region = 0; region < REGIONS_COUNT; region++)
		{
			for (int row = 0; row < data_shbn[culture][region].row_count; row++)
				if (data_shbn[culture][region].culture[row] == CULTURES_RUS[culture])
			{
				if (cultureOperations.find(data_shbn[culture][region].material_order[row].value() == cultureOperations.end()))
				{
					cultureOperations[item.operation] = item.operation;
				}
			}
		}

		for (const auto& operation : cultureOperations) {
		}

		results.push_back(cultureResult);
	}
}