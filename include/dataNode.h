#include <declaration.h>

#include <iostream>
#include <string>
#include <variant>
namespace TASQL {
//一个dataNode类负责一条记录的保存，读取，写入
class dataNode {
private:
	std::vector<std::variant<int, std::string> > _data;
	table _meta;
	std::string _src;

public:
	dataNode(std::string&& src, table& metai, void** datai) : _src(src), _meta(metai) {
		//TODO深拷贝
		for (int j = 0; j < _meta.count; j++) {
			if (_meta.col[j].type == INT) {
				_data.push_back(*(int*)datai[j]);
			} else if (_meta.col[j].type == VARCHAR) {
				_data.push_back(
					std::string((char*)datai[j]));
			}
		}
	}
	dataNode(std::string& src, table& metai) : _src(src), _meta(metai) {
		std::ifstream temp_node(_src);
		for (int j = 0; j < _meta.count; j++) {
			if (_meta.col[j].type == INT) {
				int temp;
				temp_node >> temp;
				_data.push_back(temp);
			} else if (_meta.col[j].type == VARCHAR) {
				std::string temp;
				temp_node >> temp;
				_data.push_back(temp);
			}
		}
	}
	~dataNode() {
		std::ofstream temp_node(_src);
		for (int j = 0; j < _meta.count; j++) {
			if (_meta.col[j].type == INT) {
				int temp = get<int>(_data[j]);
				temp_node << temp << " ";
			} else if (_meta.col[j].type == VARCHAR) {
				std::string temp = get<std::string>(_data[j]);
				temp_node << temp << " ";
			}
		}
	}
};	// namespace TASQL
}  // namespace TASQL