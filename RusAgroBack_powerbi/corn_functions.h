#pragma once
const std::string cornOperations[2] =
{
		u8"�������� ��� ��������� ��������� (��)",
		u8"����������� (��)"
};

std::vector<object_db> fillInputAndAlternativeOperationsForCorn(std::vector<object_db> dataCorn)
{
	std::vector<object_db> resultDataCorn;

	for (int row = 0; row < dataCorn.size(); row++)
	{
		if (dataCorn[row].material_order == cornOperations[0])
		{
			dataCorn[row].input_operation = u8"����� ��� �������� ��������� (��)";
			dataCorn[row].alt_operation = u8"����� � ��������� ��������� (��)";
			dataCorn[row].input_deadline = 3;
			dataCorn[row].alt_deadline = 3;
			resultDataCorn.push_back(dataCorn[row]);
		}
		if (dataCorn[row].material_order == cornOperations[1])
		{
			dataCorn[row].input_operation = u8"����������� (��)";
			dataCorn[row].alt_operation = u8"�������� ��� ��������� ��������� (��)";
			dataCorn[row].input_deadline = 30;
			dataCorn[row].alt_deadline = 25;
			resultDataCorn.push_back(dataCorn[row]);
		}
	}
	return resultDataCorn;
}

// �������� ������� ��� �������������� �������� �� ��������
std::vector<object_db> orderCornOperations(const std::vector<object_db>& currentDataCorn)
{
	std::string firstOrder = cornOperations[0];
	std::string secondOrder = cornOperations[1];

	std::vector<object_db> result;

	// ���������� � ���������� ��������� � �������������� ������
	std::vector<object_db> firstSlice = filter(currentDataCorn, firstOrder);
	std::vector<object_db> secondSlice = filter(currentDataCorn, secondOrder);

	result.insert(result.end(), firstSlice.begin(), firstSlice.end());
	result.insert(result.end(), secondSlice.begin(), secondSlice.end());


	return result;
}

//����������� �������� �� ��������
std::string defineCornElementStatus(object_db& elem, std::vector<object_db>& uniqCElements, std::vector<object_db>& CornUniqueSownOperations)
{
	// ���������� ��������� �������� (sownOp)
	auto sownOpIt = std::find_if(CornUniqueSownOperations.begin(), CornUniqueSownOperations.end(),
		[&elem](const object_db& el)
		{
			return el.higher_tm == elem.higher_tm && el.pu == elem.pu;
		});

	object_db sownOp = (sownOpIt != CornUniqueSownOperations.end()) ? *sownOpIt : object_db();
	std::optional<object_db> prevOp;
	std::optional<object_db> altOp;

	// ����������� prevOp � altOp � ����������� �� material_order
	if (elem.material_order == cornOperations[0])
	{
		prevOp = sownOp;
	}
	else if (elem.material_order == cornOperations[1])
	{
		auto prevOpIt = std::find_if(uniqCElements.begin(), uniqCElements.end(),
			[&elem](const object_db& el)
			{
				return el.higher_tm == elem.higher_tm && el.material_order == elem.input_operation;
			});
		prevOp = (prevOpIt != uniqCElements.end()) ? *prevOpIt : std::optional<object_db>{};
		altOp = sownOp;
	}
	else
	{
		auto prevOpIt = std::find_if(uniqCElements.begin(), uniqCElements.end(),
			[&elem](const object_db& el)
			{
				return el.higher_tm == elem.higher_tm && el.material_order == elem.input_operation;
			});
		prevOp = (prevOpIt != uniqCElements.end()) ? *prevOpIt : std::optional<object_db>{};
	}

	// ����������� ������� ��������
	if (!sownOp.is_completed)
	{
		return "not_sown";
	}
	else if (
		((prevOp.has_value() && elem.actual_data <= add_days(prevOp->actual_data, elem.input_deadline.value_or(0))) ||
			(altOp.has_value() && elem.actual_data <= add_days(altOp->actual_data, elem.alt_deadline.value_or(0)))) &&
		!elem.is_completed
		)
	{
		return "not_started";
	}
	else if (
		((prevOp.has_value() && elem.actual_data <= add_days(prevOp->actual_data, elem.input_deadline.value_or(0))) ||
			(altOp.has_value() && elem.actual_data <= add_days(altOp->actual_data, elem.alt_deadline.value_or(0)))) &&
		elem.is_completed
		)
	{
		return "in_time";
	}
	else if (
		((prevOp.has_value() && elem.actual_data <= add_days(prevOp->actual_data, 2 + elem.input_deadline.value_or(0))) ||
			(altOp.has_value() && elem.actual_data <= add_days(altOp->actual_data, 2 + elem.alt_deadline.value_or(0)))) &&
		elem.is_completed
		)
	{
		return "deviation_less_two_days";
	}
	else if (
		((prevOp.has_value() && elem.actual_data > add_days(prevOp->actual_data, 2 + elem.input_deadline.value_or(0))) ||
			(altOp.has_value() && elem.actual_data > add_days(altOp->actual_data, 2 + elem.alt_deadline.value_or(0)))) &&
		elem.is_completed
		)
	{
		return "deviation_more_two_days";
	}
	else if (
		((prevOp.has_value() && elem.actual_data > add_days(prevOp->actual_data, elem.input_deadline.value_or(0))) ||
			(altOp.has_value() && elem.actual_data > add_days(altOp->actual_data, elem.alt_deadline.value_or(0)))) &&
		!elem.is_completed
		)
	{
		return "expired_not_completed";
	}
	else
	{
		return "in_time";
	}
}

// ������� ���������� lastResult
void updateLastResultCorn(LastResult& lastResult, object_db& el, std::string& currentStatus)
{
	for (int i = 0; i < cornOperations->size(); i++)
	{
		if (el.material_order.has_value() && el.material_order.value() == cornOperations[i])
		{
			for (int j = 0; j < lastResult.corn.operations[cornOperations[i]].size(); j++)
			{
				if (lastResult.corn.operations[cornOperations[i]][j].businessDirection == el.business_dir && lastResult.corn.operations[cornOperations[i]][j].PU == el.pu)
				{
					lastResult.corn.operations[cornOperations[i]][j].statuses[currentStatus].fieldsCount++;
					lastResult.corn.operations[cornOperations[i]][j].statuses[currentStatus].fieldsSquare += el.field_square.value_or(0.0f);
					break;  // ����� �� �����, ��� ��� ������ ������� ������
				}
			}
		}
	}
}