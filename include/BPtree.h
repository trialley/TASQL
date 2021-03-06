#pragma once
#include "declaration.h"
class Btreenode {
private:
public:
	bool leaf;
	std::vector<int> pointers;
	std::vector<int> keys;
	int next_node;

public:
	void delete_key_no_leaf(int curr_node_rank, int which);

	Btreenode();
	Btreenode(bool makeleaf) {
		leaf = makeleaf;
		next_node = -1;
	}
	bool isleaf() {
		return leaf;
	}
	void set_leaf(bool val) {
		leaf = val;
	}
	int num_keys() {
		return keys.size();
	}
	int num_pointers() {
		return pointers.size();
	}
	int get_key(int i) {
		return keys[i - 1];
	}
	int get_pointer(int i) {
		return pointers[i - 1];
	}
	void set_next(int x) {
		next_node = x;
	}
	int get_next() {
		return next_node;
	}
	void push_key(int val) {
		keys.push_back(val);
	}
	void push_pointer(int val) {
		pointers.push_back(val);
	}
	void clear_data() {
		keys.clear();
		pointers.clear();
	}
	int *get_keys_add() {
		return &keys[0];
	}
	int *get_point_add() {
		return &pointers[0];
	}
	~Btreenode() {
		clear_data();
	}

	/*Function that performs binary search and returns the
       correct child. For Internal Nodes */
	int get_next_key(int search_key) {
		return (std::lower_bound(keys.begin(), keys.end(),	//返回大于或等于val的第一个元素位置。如果所有元素都小于val，则返回last的位置
								 search_key) -
				keys.begin());
	}

	/*找到这个叶子节点，看这个叶子里是不是有目标关键词 */
	int search_key(int search_key) {
		return std::binary_search(keys.begin(), keys.end(), search_key);
	}

	//插入元素需要检查是否需要分裂
	bool full() {
		if (leaf) {
			if (pointers.size() < BPTREE_MAX_KEYS_PER_NODE - 1)
				return false;
			else
				return true;
		}
		if (pointers.size() < BPTREE_MAX_KEYS_PER_NODE)
			return false;
		else
			return true;
	}
	bool toMerge() {
		if (leaf) {
			if (pointers.size() > BPTREE_MAX_KEYS_PER_NODE / 2)
				return false;
			else
				return true;
		}
		if (pointers.size() > BPTREE_MAX_KEYS_PER_NODE / 2 - 1)
			return false;
		else
			return true;
	}
	/*Function that inserts a new record in a node which
       is not yet full */
	void insert_key(int key, int point) {
		int pos = get_next_key(key);
		keys.insert(keys.begin() + pos, key);
		if (leaf)
			pointers.insert(pointers.begin() + pos, point);
		else
			pointers.insert(pointers.begin() + pos + 1, point);
	}
	void remove_v(vector<int> &v, int val) {
		vector<int>::iterator ite;
		for (ite = v.begin(); ite != v.end();) {
			if (*ite == val)
				ite = v.erase(ite);
			else
				++ite;
		}
	}
	void delete_key(int key) {
		auto pos = std::find(keys.begin(), keys.end(), key);
		pointers.erase(pointers.begin() + (pos - keys.begin()));
		keys.erase(pos);
	}
	/*Function that copies first 'n' records from a given
       node to the current node */
	void copy_first(Btreenode &node, int n) {
		keys.clear();
		pointers.clear();
		for (int i = 1; i < n; i++) {
			keys.push_back(node.get_key(i));
			pointers.push_back(node.get_pointer(i));
		}
		pointers.push_back(node.get_pointer(n));
		if (leaf) {
			keys.push_back(node.get_key(n));
		}
	}

	/*Function that copies records from a given node to
       current node. It ignores first 'n' records */
	void copy_last(Btreenode &node, int n) {
		keys.clear();
		pointers.clear();
		int lim = node.num_pointers();
		for (int i = n + 1; i <= lim; i++) {
			pointers.push_back(node.get_pointer(i));
		}
		lim = node.num_keys();
		for (int i = n + 1; i <= lim; i++) {
			keys.push_back(node.get_key(i));
		}
	}

	//Overloading write operator to write to a file
	friend std::ofstream &operator<<(std::ofstream &os, const Btreenode &en) {
		os << en.leaf << " ";
		os << (int)en.keys.size() << " ";
		for (unsigned int i = 0; i < en.keys.size(); i++) {
			os << en.keys[i] << " ";
		}
		os << (int)en.pointers.size() << " ";
		for (unsigned int i = 0; i < en.pointers.size(); i++) {
			os << en.pointers[i] << " ";
		}
		os << en.next_node;
		return os;
	}

	//从文件输入这个节点
	friend std::ifstream &operator>>(std::ifstream &is, Btreenode &en) {
		int ts;
		is >> en.leaf;
		is >> ts;  //n个关键词
		en.keys.resize(ts);
		for (int i = 0; i < ts; i++) {
			is >> en.keys[i];
		}
		is >> ts;  //n个子节点
		en.pointers.resize(ts);
		for (int i = 0; i < ts; i++) {
			is >> en.pointers[i];
		}
		is >> en.next_node;
		return is;
	}

	//Overloading Assignment Operator
	Btreenode &operator=(const Btreenode &n) {
		if (this != &n) {
			leaf = n.leaf;
			keys.assign(n.keys.begin(), n.keys.end());
			pointers.assign(n.pointers.begin(), n.pointers.end());
		}
		return *this;
	}
};

class BPtree {
private:
	int files_till_now, root_num;
	char tablename[BPTREE_MAX_FILE_PATH_SIZE];

	/* Function that writes a node 'n' to a given
       filenum(like pointer) */
	void write_node(int filenum, Btreenode n) {
		char *str;
		str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
		sprintf(str, "table/%s/tree/tree%d.dat", tablename, filenum);
		std::ofstream out_file(str, std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
		free(str);
		out_file << n;
		out_file.close();
	}

	/* Function that updates Meta-Data of a table after
       inserting a record */
	void update_meta_data() {
		char *str;
		str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
		sprintf(str, "table/%s/tree/meta_tree.dat", tablename);
		std::ofstream out_file(str, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
		out_file << files_till_now << " " << root_num;
		out_file.close();
		free(str);
	}

public:
	BPtree();

	/* Constructor function that initializes tree with existing
       meta-data or creates new tree and writes meta-data */
	BPtree(char table_name[]) {
		strcpy(tablename, table_name);
		char *str;
		str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
		sprintf(str, "mkdir -p table/%s/tree", tablename);
		system(str);
		sprintf(str, "table/%s/tree/meta_tree.dat", tablename);

		std::ifstream in_file(str, std::ifstream::in | std::ifstream::binary);
		if (!in_file) {
			/* If no meta data for table's BTree is found i.e.,
               It is new table. Then Create new meta data */
			std::ofstream out_file(str, std::ofstream::binary |
											std::ofstream::out | std::ofstream::trunc);
			if (!out_file) {
				printf("Critical Error : Unable to Write on Disk !!");
				printf("\nAborting ... ... \n");
				abort();
			}
			files_till_now = root_num = 0;
			out_file.write((char *)(&files_till_now), sizeof(files_till_now));
			out_file.write((char *)(&root_num), sizeof(root_num));
			//out_file << files_till_now << " " << root_num;
			out_file.close();
			Btreenode root(true);
			root.set_next(-1);
			write_node(0, root);
		} else {
			/* Read old Meta Data */
			in_file >> files_till_now >> root_num;	//节点数目，根节点编号
			in_file.close();
		}
		free(str);
	}

	/* Function that reads a node 'n' from a given
       filenum(like pointer) */
	void read_node(int filenum, Btreenode &n) {
		char *str;
		str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
		sprintf(str, "table/%s/tree/tree%d.dat", tablename, filenum);
		std::ifstream in_file(str, std::ifstream::in | std::ifstream::binary);
		free(str);
		in_file >> n;
		in_file.close();
	}

	int insert_record(int primary_key, int record_num);
	int delete_record(int primary_key);
	Btreenode search_leaf(int primary_key);
	int get_record(int primary_key);
	bool _borrowFromLeft();
};

/* Function that traverses the BPtree and returns the leaf node
    where 'primary_key' record should exist, if it exist at all */
Btreenode BPtree::search_leaf(int primary_key) {
	Btreenode n(true);
	int q, curr_node = root_num;
	read_node(curr_node, n);

	//Traversing the Tree from root till leaf
	while (!n.isleaf()) {
		q = n.num_pointers();
		if (primary_key <= n.get_key(1)) {
			curr_node = n.get_pointer(1);
		} else if (primary_key > n.get_key(q - 1)) {
			curr_node = n.get_pointer(q);
		} else {
			curr_node = n.get_pointer(n.get_next_key(primary_key) + 1);
		}
		read_node(curr_node, n);
	}

	return n;
}

/*
根据主键返回条目文件编号
 A function that returns the record number of a tuple
    with indexed column = 'primary_key' */
int BPtree::get_record(int primary_key) {
	clock_t start = clock();
	Btreenode n = search_leaf(primary_key);
	int pos = n.get_next_key(primary_key) + 1;
	clock_t stop = clock();
	double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
	printf("\nTime elapsed for search is %f ms", elapsed);
	if (pos <= n.num_keys() && n.get_key(pos) == primary_key) {
		return n.get_pointer(pos);
	} else {
		return BPTREE_SEARCH_NOT_FOUND;
	}
}

/* A function the inserts a (key, record_num) pair in the
    B+ Tree */
//key is first coloumn of database either can be int or varchar;
int BPtree::insert_record(int primary_key, int record_num) {
	Btreenode n(true);
	int q, j, prop_n, prop_k, prop_new, curr_node = root_num;
	bool finish = false;
	std::stack<int> S;

	read_node(curr_node, n);

	//递归获得叶子节点
	while (!n.isleaf()) {
		S.push(curr_node);							  //加入栈中以备分割
		q = n.num_pointers();						  //获得指针数量
		if (primary_key <= n.get_key(1)) {			  //小于等于倒数第一个则进入前一个节点
			curr_node = n.get_pointer(1);			  //下沉
		} else if (primary_key > n.get_key(q - 1)) {  //大于倒数第二个则进之后的节点
			curr_node = n.get_pointer(q);
		} else {
			curr_node = n.get_pointer(n.get_next_key(primary_key) + 1);	 //获取下沉位置
		}
		read_node(curr_node, n);  //更新当前节点
	}

	//Here n is Leaf Node
	//if key exist exist then return;
	if (n.search_key(primary_key)) {  //id存在于当前节点
		return BPTREE_INSERT_ERROR_EXIST;
	}

	if (!n.full()) {  //叶子节点数目小于最大-1 内节点小于最大，无需分裂，直接插入
		//leaf node empty insert here and exit
		n.insert_key(primary_key, record_num);
		write_node(curr_node, n);
		update_meta_data();
		return BPTREE_INSERT_SUCCESS;
	}

	//if node n is full, then split;
	Btreenode temp(true), new_node(true);  //构建新节点，进行分裂

	temp = n;
	temp.insert_key(primary_key, record_num);
	j = ceil((BPTREE_MAX_KEYS_PER_NODE + 1.0) / 2.0);
	n.copy_first(temp, j);
	files_till_now++;
	new_node.set_next(n.get_next());
	n.set_next(files_till_now);
	new_node.copy_last(temp, j);

	prop_k = temp.get_key(j);
	prop_new = files_till_now;
	prop_n = curr_node;
	write_node(files_till_now, new_node);
	write_node(curr_node, n);
	temp.clear_data();
	new_node.clear_data();

	/* Keep repeating until we reach root
       or find an empty internal node */
	while (!finish) {
		if (S.size() == 0) {  //如果栈为空
			/*构建根节点并重置元数据*/
			Btreenode nn(false);  //传参是不是叶节点
			nn.push_key(prop_k);
			nn.push_pointer(prop_n);
			nn.push_pointer(prop_new);
			files_till_now++;
			write_node(files_till_now, nn);
			root_num = files_till_now;
			finish = true;
		} else {
			curr_node = S.top();
			S.pop();
			read_node(curr_node, n);
			if (!n.full()) {
				n.insert_key(prop_k, prop_new);
				write_node(curr_node, n);
				finish = true;
			} else {
				/* Split is propogating towards top */
				temp = n;
				temp.insert_key(prop_k, prop_new);
				j = floor((BPTREE_MAX_KEYS_PER_NODE + 1.0) / 2.0);
				n.copy_first(temp, j);
				files_till_now++;
				new_node.set_leaf(false);
				new_node.copy_last(temp, j);
				write_node(files_till_now, new_node);
				write_node(curr_node, n);
				prop_k = temp.get_key(j);
				prop_new = files_till_now;
				prop_n = curr_node;
			}
		}
	}

	update_meta_data();
	return BPTREE_INSERT_SUCCESS;
}

int BPtree::delete_record(int primary_key) {
	Btreenode n(true);
	Btreenode father_node(true);
	int q, j, prop_n, prop_k, prop_new, curr_node = root_num;
	bool finish = false;
	std::stack<int> S;

	read_node(curr_node, n);
	int letfnode_key, rightnode_key;
	int pnode_key;
	int curr_node_rank;
	//递归获得叶子节点
	while (!n.isleaf()) {
		pnode_key = curr_node;
		S.push(curr_node);					//加入栈中以备分割
		q = n.num_pointers();				//获得指针数量
		if (primary_key <= n.get_key(1)) {	//小于等于倒数第一个则进入前一个节点
			curr_node_rank = 1;
			curr_node = n.get_pointer(1);			  //下沉
		} else if (primary_key > n.get_key(q - 1)) {  //大于倒数第二个则进之后的节点
			curr_node_rank = q;
			curr_node = n.get_pointer(q);
		} else {
			curr_node_rank = n.get_next_key(primary_key) + 1;
			curr_node = n.get_pointer(n.get_next_key(primary_key) + 1);	 //获取下沉位置
		}
		father_node = n;
		read_node(curr_node, n);  //更新当前节点
	}

	//判断叶子节点中是否含有目标节点
	if (!n.search_key(primary_key)) {  //id存在于当前节点
		std::cout << "节点中不存在目标id，无法删除" << std::endl;
		return BPTREE_INSERT_ERROR_EXIST;
	}
	std::cout << "节点中存在目标id，开始删除" << std::endl;

	//结点中关键字个数大于⌈M/2⌉，做删除操作不会破坏 B+树，
	if (!n.toMerge()) {
		//leaf node empty insert here and exit
		n.delete_key(primary_key);
		write_node(curr_node, n);
		update_meta_data();	 //更新节点数和根节点编号
		return BPTREE_INSERT_SUCCESS;
	}
	//从左兄弟借单位
	if (curr_node_rank >= 1) {
		int new_up_key;
		int new_right_key;
		int new_right_pointer;

		letfnode_key = father_node.get_pointer(curr_node_rank - 1);
		Btreenode leftnode(false);
		read_node(letfnode_key, leftnode);
		if (!leftnode.toMerge()) {	//如果可以借来
			new_right_key = leftnode.keys[leftnode.keys.size() - 1];
			new_right_pointer = leftnode.pointers[leftnode.keys.size() - 1];

			new_up_key = leftnode.keys[leftnode.keys.size() - 2];

			leftnode.delete_key(new_right_key);	 //删除借走的key
			write_node(letfnode_key, leftnode);

			n.keys.insert(n.keys.begin(), new_right_key);  //加上拿到的key
			n.pointers.insert(n.pointers.begin(), new_right_pointer);

			n.delete_key(primary_key);
			write_node(curr_node, n);

			father_node.keys[curr_node_rank - 2] = new_up_key;	//父节点更新
			write_node(pnode_key, father_node);
			return BPTREE_INSERT_SUCCESS;
		}
	}
	// //从右兄弟借单位
	if (curr_node_rank < father_node.pointers.size()) {
		int new_up_key;
		int new_left_key;
		int new_left_pointer;

		rightnode_key = father_node.get_pointer(curr_node_rank + 1);
		Btreenode rightnode(false);
		read_node(rightnode_key, rightnode);
		if (!rightnode.toMerge()) {	 //如果可以借来
			new_left_key = rightnode.keys[0];
			new_left_pointer = rightnode.pointers[0];

			new_up_key = rightnode.keys[0];

			rightnode.delete_key(new_left_key);	 //删除借走的key
			write_node(rightnode_key, rightnode);

			n.keys.insert(n.keys.end(), new_left_key);	//加上拿到的key
			n.pointers.insert(n.pointers.end(), new_left_pointer);

			n.delete_key(primary_key);
			write_node(curr_node, n);

			father_node.keys[curr_node_rank - 1] = new_up_key;	//父节点更新
			write_node(pnode_key, father_node);
			return BPTREE_INSERT_SUCCESS;
		}
	}

	// //合并节点
	if (curr_node_rank < father_node.pointers.size()) {	 //有右
		rightnode_key = father_node.get_pointer(curr_node_rank + 1);
		Btreenode rightnode(false);
		read_node(rightnode_key, rightnode);

		n.keys.insert(n.keys.end(), rightnode.keys.begin(), rightnode.keys.end());
		n.pointers.insert(n.pointers.end(), rightnode.pointers.begin(), rightnode.pointers.end());
		n.set_next(-1);
		write_node(curr_node, n);

		// remove("table/ss/tree/tree" + std::to_string(rightnode_key) + ".dat")

		father_node.delete_key_no_leaf(curr_node_rank, +1);	 //删除两子节点中间的分割id
		write_node(pnode_key, father_node);

	} else if (curr_node_rank >= 1) {  //有左
		letfnode_key = father_node.get_pointer(curr_node_rank - 1);
		Btreenode leftnode(false);
		read_node(letfnode_key, leftnode);

		n.keys.insert(n.keys.begin(), leftnode.keys.begin(), leftnode.keys.end());
		n.pointers.insert(n.pointers.begin(), leftnode.pointers.begin(), leftnode.pointers.end());
		write_node(curr_node, n);

		father_node.delete_key_no_leaf(curr_node_rank, -1);	 //删除两子节点中间的分割id
		write_node(pnode_key, father_node);
	}

	update_meta_data();
	return BPTREE_INSERT_SUCCESS;
}

void Btreenode::delete_key_no_leaf(int curr_node_rank, int which) {
	if (which == 1) {
		keys.erase(keys.begin() + curr_node_rank - 1);
		pointers.erase(pointers.begin() + curr_node_rank - 1 + 1);
	} else {
		keys.erase(keys.begin() + curr_node_rank - 2);
		pointers.erase(pointers.begin() + curr_node_rank - 2);
	}

	return;
}