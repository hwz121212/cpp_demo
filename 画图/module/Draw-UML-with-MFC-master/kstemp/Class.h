#include"base.h"
#include<list>
using namespace std;
class Class:public base
{
protected:
	int part1,part2;
	CString name;
	list<CString>vlist;
	list<CString>flist;
	
public:
	Class(int l,int u,int r,int d,int p1,int p2);
	int onpressLB(int x,int y);
	int DblClk(CPoint point);
	void onmove(int cx,int cy);
	void ondraw(CDC *pDC);
	void setName(CString str);//设置类名
	void setV(CString str);
	void setF(CString str);
	void addline(base* b);
	void deleteline(base* b);
	int size();
	base* getend();
	base* getstart();
	void setend(base* b);
	void setstart(base* b);
	//list<base*> getlines();
	//void output(ostream &out);                  以后再做
};
