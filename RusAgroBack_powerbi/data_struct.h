#pragma once
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

    Operation(const std::string& businessDirection, const std::string& PU) : businessDirection(businessDirection), PU(PU) 
    {
        statuses["not_sown"] = { 0, 0 };
        statuses["not_started"] = { 0, 0 };
        statuses["in_time"] = { 0, 0 };
        statuses["deviation_less_two_days"] = { 0, 0 };
        statuses["deviation_more_two_days"] = { 0, 0 };
        statuses["expired_not_completed"] = { 0, 0 };
    }
};

struct Crop
{
    std::unordered_map<std::string, std::vector<Operation>> operations;
};

struct LastResult
{
    Crop sugar_beet;
    Crop corn;
    Crop soy;
    Crop sunflower;
    Crop corn_silage;
};

// Функция для конвертации структур в JSON и записи в файл
void writeReportToJson(const LastResult& lastResult, const std::string& filename)
{
    nlohmann::json jsonReport;

    auto cropToJson = [](const Crop& crop) {
        nlohmann::json jCrop;
        for (const auto& operationPair : crop.operations)
        {
            nlohmann::json jOperations;
            for (const auto& operation : operationPair.second)
            {
                nlohmann::json jOperation;
                jOperation["businessDirection"] = operation.businessDirection;
                jOperation["PU"] = operation.PU;

                nlohmann::json jStatuses;
                for (const auto& statusPair : operation.statuses)
                {
                    nlohmann::json jStatus;
                    jStatus["fieldsCount"] = statusPair.second.fieldsCount;
                    jStatus["fieldsSquare"] = statusPair.second.fieldsSquare;
                    jStatuses[statusPair.first] = jStatus;
                }
                jOperation["statuses"] = jStatuses;
                jOperations.push_back(jOperation);
            }
            jCrop[operationPair.first] = jOperations;
        }
        return jCrop;
        };

    // Преобразуем каждую культуру в JSON
    jsonReport["sugar_beet"] = cropToJson(lastResult.sugar_beet);
    jsonReport["corn"] = cropToJson(lastResult.corn);
    jsonReport["soy"] = cropToJson(lastResult.soy);
    jsonReport["sunflower"] = cropToJson(lastResult.sunflower);
    jsonReport["corn_silage"] = cropToJson(lastResult.corn_silage);

    // Запись JSON в файл
    std::ofstream file(filename);
    if (file.is_open())
    {
        file << jsonReport.dump(4); // Печать с отступом в 4 пробела для удобства чтения
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void printStatus(const std::string& statusName, const Status& status) 
{
    std::cout << "    Status: " << statusName << "\n";
    std::cout << "        Fields Count: " << status.fieldsCount << "\n";
    std::cout << "        Fields Square: " << status.fieldsSquare << "\n";
}

void printOperation(const Operation& operation) 
{
    std::cout << "  Business Direction: " << operation.businessDirection << "\n";
    std::cout << "  PU: " << operation.PU << "\n";
    for (const auto& statusPair : operation.statuses) 
    {
        printStatus(statusPair.first, statusPair.second);
    }
}

void printCrop(const std::string& cropName, const Crop& crop) 
{
    std::cout << "Crop: " << cropName << "\n";
    for (const auto& operationPair : crop.operations) 
    {
        std::cout << " Operation: " << operationPair.first << "\n";
        for (const auto& operation : operationPair.second) 
        {
            printOperation(operation);
        }
    }
    std::cout << std::endl;
}

void printLastResult(const LastResult& lastResult) 
{
    printCrop("Sugar Beet", lastResult.sugar_beet);
    printCrop("Corn", lastResult.corn);
    printCrop("Soy", lastResult.soy);
    printCrop("Sunflower", lastResult.sunflower);
    printCrop("Corn Silage", lastResult.corn_silage);
}