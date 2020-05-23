
#include <CJsonObject.h>
#include <declaration.h>

#include <exception>
#include <filesystem>
#include <iterator>
#include <string>
class TASQL {
public:
	//CRUD
	// key
	bool create(neb::CJsonObject& in) {
	}

	bool update(neb::CJsonObject& in) {
	}
	bool search(neb::CJsonObject& in) {
	}
	bool deletei(neb::CJsonObject& in) {
	}

public:
	TASQL() {
	}
	TASQL(std::string& pathi) {
		_checkAndCreate(pathi);
		_root_path = pathi;
		displayHelp();
		displayDataBase();
	}
	void displayDataBase() {
		for (auto& name : std::filesystem::directory_iterator(_root_path)) {
			if (name.is_directory()) {
				LOG << "find dir:" << name.path().filename() << std::endl;
			} else {
				LOG << "database root dir should not have file." << std::endl;
			}
		}
	}
	void displayHelp() {
		LOG << R"(
这里是TASQL

)";
	}

private:
	std::string _root_path;
	void _checkAndCreate(std::string& pathi) {
		// std::filesystem::path tmep_path(pathi);
		if (!std::filesystem::exists(pathi)) {
			LOG << "该路径不存在，即将创建\n";
			std::filesystem::create_directory(pathi);
			LOG << "创建数据库文件夹完毕\n";
		} else {
			std::filesystem::directory_entry entry(pathi);	//文件入口
			if (!entry.is_directory()) {
				LOG << "该路径不是文件夹\n";
				throw std::invalid_argument("该路径不是文件夹");
			}
		}
	}
};