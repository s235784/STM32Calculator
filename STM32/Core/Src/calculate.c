#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define M_E 2.718281828459
#define M_PI 3.141592653589
#define INPUT_MAX 20
#define NODE_MAX 20
#define NUM_OPT 16
#define LEN_OPT 6
#define NUM_OPS 10

typedef enum {
	Opd = 0,
	Opt = 1
}Type;
typedef struct {
	int opt;
	double opd;
	Type tp;
}Node;

//???,????????
char brackets[NUM_OPS];
char info[INPUT_MAX];
int bb = 0;

int input(Node* in, char *ins);
void cutFloatEndZero(double fVal, char* poutval);
int translate(char* p, Node* re, int* len);
double translateopd(char* p, int* len);
int translateopt(char* p, int* len);
int cmp(const char* s, const char* d);
int calculate(Node* n, double* r);
int sclt(int opr, double* opd);
int dclt(int opr, double* opd1, double opd2);
int prid(int c);

/*??:????????
 *      ?????????????????????
 *??:???????
 *??:????,????1,????0
 */
int input(Node* in, char *ins)
{
	int insi = 0;
	int len = 0;
	//??????
	while (ins[insi])
	{
		if (translate(ins + insi, in++, &len))
			insi += len;
		else
			return 0;
	}
	//??????
	if ((in - 1)->opt != '=')
		in->opt = '=', in->tp = Opt, in++;
	in->opt = -1, in->tp = Opt;
	return 1;
}
/*??:?????????????
 *??:?????,??????,?????????????
 *??:??????
 */
int translate(char* p, Node* re, int* len)
{
	if (*p >= '0' && *p <= '9')
	{
		re->tp = Opd;
		re->opd = translateopd(p, len);
		return 1;
	}
	else if (*p == 'e' || *p == 'p')
	{
		if (*p == 'e')
		{
			re->tp = Opd;
			re->opd = M_E;
			*len = 1;
			if (p[1] == ' ')
				(*len)++;	//????,???????
			return 1;
		}
		else
		{
			if (p[1] == 'i')
			{
				re->tp = Opd;
				re->opd = M_PI;
				*len = 2;
				if (p[2] == ' ')
					(*len)++;
				return 1;
			}
			else
				return 0;
		}
	}
	else
	{
		re->tp = Opt;
		re->opt = translateopt(p, len);
		if (re->opt)
			return 1;
		else
			return 0;
	}
}
/*??:?????
 */
double translateopd(char* p, int* len)
{
	int flag = 0;
	int pointnum = 1;
	double temp = 0;
	int i = 0;
	do {
		if (!flag && p[i] != '.')
			temp = temp * 10 + p[i++] - '0';
		else if (!flag && p[i] == '.')
			flag = 1, i++;
		else
			temp += (p[i++] - '0') * pow(0.1, pointnum), pointnum++;
	} while ((p[i] >= '0' && p[i] <= '9') || p[i] == '.');
	if (p[i] == ' ')
		i++;
	*len = i;
	return temp;
}
/*??:?????
 *      ???????,???0,?????????
 */
int translateopt(char* p, int* len)
{
	char fu[NUM_OPT][LEN_OPT] = { "+","-","*","/","^","(",")","sin","cos","tan","asin","acos","atan","ln","lg","=" };
	int fu_int[NUM_OPT] = { '+','-','*','/','^','(',')','s','c','t','s' + 'a','c' + 'a','t' + 'a','n','g','=' };
	int i = 0;
	for (i = 0; i < NUM_OPT; i++)
	{
		if (*len = cmp(p, fu[i]))
			break;
	}
	if (i == NUM_OPT)
		return 0;
	if (p[*len] == ' ')
		(*len)++;
	return fu_int[i];
}
/*??:?????,??????
 *      ?????0
 */
int cmp(const char* s, const char* d)
{
	char* dd = (char*)d;
	while (*d)
		if (*(d++) != *(s++))
			return 0;
	return d - dd;
}
/*??:????
 *??:??????,????????
 *??:????
 */
int calculate(Node* n, double* r)
{
	//???
	double OPRD[NUM_OPS] = { 0 };
	int OPRT[NUM_OPS] = { 0 };
	int db = 0;
	int tb = 0;

	int top = 0;
	double a = 0.0, b = 0.0;
	int o = 0;
	int len = 0;
	Node* nb = n;

	if (n->tp == Opt && n->opt == '-')
		OPRD[db] = 0.0, db++;	//Push(&OPRD,0.0); 
	while (1)
	{
		//?????????
		if (n->tp == Opd)
		{
			OPRD[db] = n->opd, db++, n++;	//Push(&OPRD,n->opd),n++;
		}
		else
		{
			//?????
			if (prid(n->opt))
			{
				//if(!GetTop(OPRT,&top))
				if (tb)
					top = OPRT[tb - 1];
				if (!tb)
				{
					if (n->opt == '=' || n->opt == ')')
					{
						if (n->opt == ')')
						{
							bb--;	//if(!Pop(&brackets,&temp))
							if (bb < 0)
							{
								strcpy(info, "Error Bracket");
								return 0;
							}
						}
						break;
					}
					else
						OPRT[tb] = n->opt, tb++, n++;	//Push(&OPRT,n->opt),n++;
					continue;
				}
				if (prid(top) == 0)
				{
					//Pop(&OPRD,&a);
					//Pop(&OPRT,&o);
					db--;
					a = OPRD[db];
					tb--;
					o = OPRT[tb];
					if (sclt(o, &a))
					{
						OPRD[db] = a, db++;	//Push(&OPRD,a);
						continue;
					}
					else
						return 0;
				}
				if (prid(top) >= prid(n->opt))
				{
					//Pop(&OPRD,&b);
					//Pop(&OPRD,&a);
					//Pop(&OPRT,&o);
					db--;
					b = OPRD[db];
					db--;
					a = OPRD[db];
					tb--;
					o = OPRT[tb];
					if (dclt(o, &a, b))
					{
						OPRD[db] = a, db++;	//Push(&OPRD,a);
					}
					else
						return 0;
				}
				else
				{
					OPRT[tb] = n->opt, tb++, n++;	//Push(&OPRT,n->opt),n++;
				}
			}
			else
			{
				//????????
				double x = 0.0;
				if (n->opt == '(')
				{
					brackets[bb] = '(', bb++;	//Push(&brackets,'(');
					if (len = calculate(n + 1, &x))
					{
						OPRD[db] = x, db++;	//Push(&OPRD,x);
						n += len, n++;
						if (n->tp == Opt && n->opt == -1)
						{
							strcpy(info, "Error Bracket");
							return 0;
						}
					}
					else
						return 0;
				}
				else
				{
					OPRT[tb] = n->opt, tb++;	//Push(&OPRT,n->opt);
					n++;
				}
			}
		}
	}
	*r = OPRD[db - 1];
	return n - nb + 1;
}
/*??:????
 */
int sclt(int opr, double* opd)
{
	switch (opr)
	{
	case 's':
		*opd = sin(*opd);
		break;
	case 'c':
		*opd = cos(*opd);
		break;
	case 't':
		*opd = tan(*opd);
		break;
	case 'a' + 's':
		if (*opd < -1 || *opd>1)
		{
			strcpy(info, "Beyond asin()");
			return 0;
		}
		else
			*opd = asin(*opd);
		break;
	case 'a' + 'c':
		if (*opd < -1 || *opd>1)
		{
			strcpy(info, "Beyond acos()");
			return 0;
		}
		else
			*opd = acos(*opd);
		break;
	case 'a' + 't':
		if (*opd > -3.141592654 / 2 && *opd < 3.141592654 / 2)
			*opd = atan(*opd);
		else
		{
			strcpy(info, "Beyond atan()");
			return 0;
		}
		break;
	case 'n':
		if (*opd > 0)
			*opd = log(*opd);
		else
		{
			strcpy(info, "Beyond ln()");
			return 0;
		}
		break;
	case 'g':
		if (*opd > 0)
			*opd = log10(*opd);
		else
		{
			strcpy(info, "Beyond lg()");
			return 0;
		}
		break;
	}
	return 1;
}
/*??:????
 */
int dclt(int opr, double* opd1, double opd2)
{
	switch (opr)
	{
	case '+':
		*opd1 = *opd1 + opd2; break;
	case '-':
		*opd1 = *opd1 - opd2; break;
	case '*':
		*opd1 = (*opd1) * opd2; break;
	case '/':
		if (opd2 > pow(0.1, 8) || opd2 < 0 - pow(0.1, 8))
			*opd1 = (*opd1) / opd2;
		else
		{
			strcpy(info, "Error 1/0");
			return 0;
		}
		break;
	case '^':
		*opd1 = pow(*opd1, opd2); break;
	}
	return 1;
}
/*??:?????
 */
int prid(int c)
{
	switch (c)
	{
	case '+':
	case '-':
	case '=':
	case ')':
		return 1;
	case '*':
	case '/':
		return 2;
	case '^':
		return 3;
	default:
		return 0;
	}
}

/* ??:???????0
*/
void cutFloatEndZero(double fVal, char* poutval)
{
	int i;
	int nCnt = 0;
	int nlen = 0;
	char abyVal[64];
	sprintf(abyVal, "%lf", fVal);
	nlen = strlen(abyVal);

	if (*(abyVal + nlen - 1) != '0')
	{
		sprintf(poutval, "%lf", fVal);
		return;
	}

	for (i = nlen - 1; i > 0; i--)
	{
		if (*(abyVal + i) == '0')
		{
			nCnt++;
		}
		else
		{
			break;
		}
	}
	sprintf(poutval, "%.*lf", 6 - nCnt, fVal);
}

int startCalculate(char *tempStr)
{
	strcpy(info, "");
	char insi[INPUT_MAX];
	strcpy(insi, tempStr);
	Node nodes[NODE_MAX] = { 0 };
	double r = 0.0;
	//?????
	bb = 0;
	//??
	if (!input(nodes, insi))
	{
		printf("t1.txt=\"Input Error\"\xff\xff\xff");
		return 0;
	}
	//??
	if (calculate(nodes, &r))
	{
		if (bb)
		{
			printf("t1.txt=\"Error Bracket\"\xff\xff\xff");
			return 0;
		}
		
		if (info[0] == '\0') {
			char result[INPUT_MAX];
			cutFloatEndZero(r, result);
			printf("t1.txt=\"%s\"\xff\xff\xff", result);
			return 0;
		} else {
			printf("t1.txt=\"%s\"\xff\xff\xff", info);
		}
		
		return 0;
	}
	if (info[0] == '\0') {
		printf("t1.txt=\"Error NULL\"\xff\xff\xff");
	} else {
		printf("t1.txt=\"%s\"\xff\xff\xff", info);
	}
	
	return 0;
}
