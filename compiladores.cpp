#include <iostream>
#include <queue>
#include <list>
#include <algorithm>
#include <fstream>
#include <tuple>

using namespace std;

list<int>  list_estados;
list< tuple<int, int, int> > list_transc;
list<int> list_estado_acept;
list<int> list_entradas;


list<int> eClausura(list<int> *R)
{
	list<int> e_clausura; 
	queue<int> myqueue;

    for_each(R->begin(),R->end(),[& e_clausura,& myqueue](int i)
    {
        myqueue.push(i);
        e_clausura.push_back(i);
    });
    while (!myqueue.empty())
    {
		int r=myqueue.front();
        myqueue.pop(); 
		for_each(list_transc.begin(),list_transc.end(),[r,&e_clausura,&myqueue](tuple<int,int,int> transc){
        	if(get<0>(transc) == r && get<1>(transc) == -1 )
        	{
        		int s=get<2>(transc);
        		e_clausura.push_back(s);
        		myqueue.push(s);
        	}
        });
    }
    return e_clausura;
}


void print(list<int> e)
{
	for_each(e.begin(),e.end(),[](int i){
		cout<<i<<" ";
	});
	cout<<endl;
}


list<int> mover(list<int> R,int x)
{
	list<int> result;
	
	for_each(R.begin(),R.end(),[ & result, x ](int _r){
		for_each(list_transc.begin(),list_transc.end(),[&result,x,_r](tuple<int, int, int> transc){			
			if(get<0>(transc) == _r and get<1>(transc) == x )
				result.push_back(get<2>(transc));
		});	
	});
	return result;
}


bool existe(list<list<int>> _listOfList ,list<int> _list)
{
	for_each(_listOfList.begin(),_listOfList.end(),[_list](list<int> lst ){
		print(lst);
		print(_list);
		if(equal(_list.begin(),_list.end(),lst.begin()))
		{
			cout<<"Iguales"<<endl;
			cout<<"----------------------"<<endl;
			return true;
		}
		cout<<"----------------------"<<endl;
		

		
	
	});
	return false;
}


void getAFD()
{
	int entradaInt=list_estados.front(); 
	list<int> R;
	list<int> entrada;
	queue<list<int>> Rmarcados;
	
	entrada.push_back(entradaInt);
	
	list<list<int>>  D_est;
	list<int> U_Ptr= eClausura(&entrada);
	D_est.push_back(eClausura(&entrada));

	for_each(U_Ptr.begin(),U_Ptr.end(),[&R](int i){
		R.push_back(i);		
	});
	

	Rmarcados.push(R);
	
	while(!Rmarcados.empty())
	{
		list<int> temp_R;
		
		for_each(list_entradas.begin(),list_entradas.end(),[&Rmarcados,&D_est,&temp_R](int x){

			list<int> tmpMoved=mover(Rmarcados.front(),x);
			list<int> U=eClausura(&tmpMoved);
			if(!U.empty() && ! existe( D_est,U ))
			{	
				U.sort();
				D_est.push_back(U);
				Rmarcados.push(U);
				copy ( U.begin(), U.end() , temp_R.begin() );
			}
		});

		Rmarcados.pop();

	}


	for_each(D_est.begin(),D_est.end(),[](list<int> lst){
		print(lst);
	});

	
		
}



int main()
{
    fstream iFile;
	iFile.open ("in.txt", std::fstream::in );

    int num_E;
	int numOf;
	int estado;
	string buffer;   

    
	iFile>>buffer;
    iFile>>numOf;

    for(int i=0;i<numOf;i++)
    {
        iFile>>estado;
        list_estados.push_back(estado);
    }


    iFile>>buffer;
    iFile>>numOf;

    for(int i=0;i<numOf;i++)
    {
        iFile>>estado;
        list_estado_acept.push_back(estado);
    }
    
    iFile>>buffer;
    iFile>>numOf;
    
    for(int i=0;i<numOf;i++)
    {
        iFile>>estado;
        list_entradas.push_back(estado);
    }
    
    iFile>>buffer;
    iFile>>numOf;
    
    for(int i=0;i<numOf;i++)
    {
        tuple<int, int, int> tpl;
        iFile>>get<0>(tpl);
        iFile>>get<1>(tpl);
        iFile>>get<2>(tpl);
        list_transc.push_back(tpl);
    }
		
	list<int> *R=new list<int>;
	R->push_back(1);
	R->push_back(6);
	
	eClausura(R);
	
	getAFD();


    return 0;
}
