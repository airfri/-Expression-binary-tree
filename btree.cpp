
#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

const int MAXSIZE = 100;

//全局变量
char formula[MAXSIZE];//存放输入的表达式
bool is_number[MAXSIZE];//判断每个字符是否为数字，是为true,不是为false
char ops[MAXSIZE];//存放读入的操作符
double num[MAXSIZE];//存放读入的数字

//定义树类
class BTNode 
{
public:
	BTNode* leftChild;	//左孩子
	BTNode* rightChild;	//右孩子
	char op;			//操作符
	double value;		//操作数
	//构造函数：初始化所有成员变量
	BTNode()
	{
		leftChild = NULL;
		rightChild = NULL;
		op = '#';//操作数初始化为#
		value = 0;
	}
};

//（1）前缀表达式建树：叶子存数值，分支存运算符
//从后往前遍前缀表达式，每遇到一个运算符就用它作为根节点,其后面两个数据或节点作为其左右孩子
BTNode* pre_order_build(bool *is_number, int len)
{
	BTNode *T = new BTNode;

	BTNode *p_tree[MAXSIZE];//指针数组
	for (int i = 0; i < len; i++) 
		p_tree[i] = new BTNode;

	int *p_status = new int[MAXSIZE];//指针数组中每个元素的状态，0为操作符||不可选，1为操作数，2为已经连接的节点（子树）
	for (int i = 0; i < len; i++) 
	{
		if (is_number[i]) 
			p_status[i] = 1;//为数字
		else 
			p_status[i] = 0;//为操作符
	}
	if (len == 1) //只有一个元素，则直接赋值
		p_tree[0]->value = num[0];
	for (int i = len - 1; i >= 0; i--) 
	{
		if (p_status[i] == 0)  //如果为操作符，创建节点
		{ 
			p_tree[i]->op = ops[i];			//存入操作符
			int sum_child = 2;//判断当前节点是否已经连接左右子节，2为当前值为左
			for (int j = i + 1; j < len; j++) 
			{
				if (p_status[j] == 1) //当前位置是数字
				{
					if (sum_child == 2) //有2个孩子
					{
						p_tree[i]->leftChild = p_tree[j];//节点后面两个分别为左边孩子和右孩子
						p_tree[j]->value = num[j];
					}
					if (sum_child == 1) //
					{
						p_tree[i]->rightChild = p_tree[j];//节点后面两个分别为左边孩子和右孩子
						p_tree[j]->value = num[j];
					}
					p_status[j] = 0;//设置为不可选
					sum_child--;
				}
				if (p_status[j] == 2) //当前位置节点（子树）
				{
					if (sum_child == 2) 
						p_tree[i]->leftChild = p_tree[j];
					if (sum_child == 1) 
						p_tree[i]->rightChild = p_tree[j];
					p_status[j] = 0;//设置为不可选
					sum_child--;
				}
				if (sum_child == 0) j = len;
			}
			p_status[i] = 2;//2表示当前位置置为节点(子树)
		}
	}
	T = p_tree[0];
	return T;
}

//（2）中序遍历输出二叉树
void in_order_output(BTNode* node)
{
	int Bracket_num = 0;
	if (node == NULL) return;//如果节点为空则返回
	if (node->op == '+' || node->op == '-') //加减号则加上括号
	{
		cout << "(";
		Bracket_num++;
	}
	in_order_output(node->leftChild);//遍历左

	if (node->op != '#')			//如果当前节点不是'#'
		cout << node->op << " ";	//则输出操作符
	else 
		cout << node->value << " ";//否则输出操作数

	in_order_output(node->rightChild);//遍历右
	if (Bracket_num)
	{
		cout << ")";
		Bracket_num--;
	}
}

//（3）对变量赋值：将表达式中的变量替换为常数
void assin(char *str, char *var_na, int *var_va, int len1, int len2)
{
	for (int i = 0; i < len1; i++)
		if ((str[i] >= 'a'&&str[i] <= 'z') || (str[i] >= 'A'&&str[i] <= 'Z'))
		{
			for (int j = 0; j < len2; j++)
				if (str[i] == var_na[j])
				{
					str[i] = '0' + var_va[j];
				}

		}
}

//（4）后序遍历对算数表达式求值
double calculate(BTNode* node)
{
	if (node->leftChild == NULL && node->rightChild == NULL)
	{
		return node->value;//如果左右孩子都为空，则返回根节点的值
	}
	double sub1 = calculate(node->leftChild);//左子树求值
	double sub2 = calculate(node->rightChild);//右子树求值
	
	switch (node->op)
	{
	case '+':
		node->value = sub1 + sub2;//加
		break;
	case '-':
		node->value = sub1 - sub2;//减
		break;
	case '*':
		node->value = sub1 * sub2;//乘
		break;
	case '/':
		node->value = sub1 / sub2;//除
		break;
	default:
		cout << "识别到错误的运算符:" << "\"" << node->op << "!\"" << endl;
	}

	return node->value;
}


//将输入的表达式转换成字符或数值
void transform(char *str, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (str[i] >= '0'&&str[i] <= '9')
		{
			num[i] = str[i] - '0';
			is_number[i] = 1;//1代表数字
		}
		else
		{
			ops[i] = str[i];
			is_number[i] = 0;//0代表字符
		}
	}
}

//输出表达式
void formula_output(int len)
{
	for (int i = 0; i < len; i++)
	{
		if (is_number[i])
			cout << num[i];
		else cout << ops[i];
	}
}

//不含变量的表达式
void opt1() 
{
	cout << "输入待计算的前缀表达式(注：请不要输入中文括号)" << endl;
	cin >> formula;			//从键盘读入算数表达式
	int len = strlen(formula);	//获取字符长度
	transform(formula, len);
	BTNode *root = pre_order_build(is_number, len);
	if (root != NULL) {
		in_order_output(root);//遍历结果
		cout << "=" << calculate(root) << endl;
	}
	else 
	{
		cout << "建树失败,请检查输入的表达式!" << endl;
	}
	system("pause");
}

//含变量的表达式
void opt2() 

{
	int var_num;//变量的个数
	cout << "输入变量的个数(请用单个字母a~z表示变量名)：";
	cin >> var_num;
	char var_na[MAXSIZE];//变量名数组
	int var_va[MAXSIZE];//变量值数组
	for (int i = 0; i < var_num; i++) 
	{
		cout << "输入第" << i << "个变量名："; cin >> var_na[i];
		cout << "输入第" << i << "个变量值："; cin >> var_va[i];
	}
	cout << "输入待计算的表达式(注：不要输入中文括号)" << endl;
	cin >> formula;			//输入字符串表达式
	int len = strlen(formula);	//字符串长度
	assin(formula, var_na, var_va, len, var_num);//为变量赋值
	transform(formula, len);
	//formula_output();//输出表达式
	BTNode *root = pre_order_build(is_number, len);
	if (root != NULL) 
	{
		in_order_output(root);//中序遍历输出表达式
		cout << "=" << calculate(root) << endl;//输出计算结果
	}
	else 
	{
		cout << "建树失败,请检查输入的表达式!" << endl;
	}
	system("pause");
}

//菜单
void text_menu() 
{
	cout << "——————————————————" << endl;
	cout << "|	表达式建树：计算表达式的值 |" << endl;
	cout << "|	1.不含变量的表达式	   |" << endl;
	cout << "|	2.含有变量的表达式	   |" << endl;
	cout << "|	3.退出          	   |" << endl;
	cout << "——————————————————\n" << endl;
	cout << "请输入选项（1~3）：";

	int select;
	while (scanf_s("%d", &select) != NULL)
	{
		switch (select)
		{
		case 1:opt1(); break;
		case 2:opt2(); break;
		case 3:exit(1); break;
		default:
			cout << "输入错误,请重新输入！" << endl;
		}
		cout << "——————————————————" << endl;
		cout << "|	表达式建树：计算表达式的值 |" << endl;
		cout << "|	1.不含变量的表达式	   |" << endl;
		cout << "|	2.含有变量的表达式	   |" << endl; 
		cout << "|	3.退出          	   |" << endl;
		cout << "——————————————————\n" << endl;
		cout << "请输入选项（1~3）：";
	}
}

//主函数
int main()
{
	text_menu();
	system("cls");
	return 0;
}
