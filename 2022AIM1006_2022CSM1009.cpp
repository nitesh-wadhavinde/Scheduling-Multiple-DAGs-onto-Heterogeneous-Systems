#include <bits/stdc++.h>
using namespace std;


int Number_of_Graphs = 2;
int Max_Number_of_Processors = 5;
vector <pair <int,int >> Ready_Queue[100];
vector<pair < double ,int > >Slow_Down;
vector<pair <int , int >> Make_Span;
int Completion_TIme;
vector<double> SD(2);


class DAG
{
public:
vector <int> Parent_Number[50];
vector<int> child[40];
vector < pair <double, int> > rank_task;
int Execution_Cost_of_Node[70][60];
int graph_matrix[30][30],tasks,proc;
double Rank_of_node[200],Average[70];
int Earliest_Start_Time[70][70],Early_Finish_Time[70][70],Actual_Finish_Time[70];
int Processor_Taking_Minimum_Time[70];
};
DAG Dag_Number[2];

class processor_slot
{
	public:
        int d;
		int task_no;
		int start_time;
		int end_time;

};

vector <processor_slot> processor_scheduler[100];



void Edge_Addition(int Graph_number, int i, int j , int k)
{
    DAG& dag = Dag_Number[Graph_number];
    dag.graph_matrix[i][j] = k;
    vector<int>& child = dag.child[i];
    child.push_back(j);
    vector<int>& parents_of_j = dag.Parent_Number[j];
    parents_of_j.push_back(i);




}

bool sortBySecond(const std::pair<int, int>& Processor_Title, const std::pair<int, int>& Finish_Time_Processor) {
    return Processor_Title.second < Finish_Time_Processor.second;
}


void Edge_Additions()
{
	//add edges
	Edge_Addition(0, 0, 1, 2);
	Edge_Addition(0, 0, 2, 3);
	Edge_Addition(0, 1, 3, 4);
	Edge_Addition(0, 1, 4, 4);
	Edge_Addition(0, 2, 3, 3);
	Edge_Addition(0, 2, 4, 2);
    Edge_Addition(1, 1, 4, 6);
	Edge_Addition(1, 2, 5, 2);
	Edge_Addition(1, 2, 4, 3);
	Edge_Addition(1, 3, 5, 2);
	Edge_Addition(1, 4, 6, 5);
	Edge_Addition(1, 5, 6, 2);
	Edge_Addition(1, 0, 1, 2);
	Edge_Addition(1, 0, 2, 4);
	Edge_Addition(1, 0, 3, 3);

}


int Calculation_Time_Estimated(int node, int processor, int dag) {
    int Time_Taken_Communication;
    int Parent_Node;
    int max_time = 0;
    int Time_Temprary = 0;

    // Error handling
    if (node < 0 || node >= Dag_Number[dag].proc) {

        return -1;
    }
    int k=0;
    while(k < Dag_Number[dag].Parent_Number[node].size())
    {
        k=k+1;
        DAG& current_dag = Dag_Number[dag];
        int parent_node = current_dag.Parent_Number[node][0];

        if (current_dag.Processor_Taking_Minimum_Time[parent_node] == processor) {
        Time_Temprary = current_dag.Actual_Finish_Time[parent_node];
        }
        else {
        int Time_Taken_Communication = current_dag.graph_matrix[parent_node][node];
        Time_Temprary = Time_Taken_Communication + current_dag.Actual_Finish_Time[parent_node];
        }
        max_time = max(max_time, Time_Temprary);
    }
    return max_time;
}


double Rank_Calculation(int i, int dag) {
    double temp;
    double maxi = 0;
    DAG& current_dag = Dag_Number[dag];
    // Error handling
    if (i < 0 || i >= current_dag.proc) {

        return -1;
    }

    int j=0;
    while(j<current_dag.proc)
    {
        if (current_dag.graph_matrix[i][j] != -1) {
            temp = current_dag.Rank_of_node[j];

           int communication_time = current_dag.graph_matrix[i][j];
           int total_time = communication_time + temp;

           if (maxi < total_time)
             {
            maxi = total_time;
            }

        }
        if (current_dag.Rank_of_node[j] == -1) {
            Rank_Calculation(j, dag);
        }


        j=j+1;
    }
    current_dag.Rank_of_node[i] = current_dag.Average[i] + maxi;
    return current_dag.Rank_of_node[i];
}

int Early_Finish_Time_cal(int i, int j, int dag)
{
    int start_time;

    if (processor_scheduler[j].empty())
     {
        int tot=Dag_Number[dag].Earliest_Start_Time[i][j] + Dag_Number[dag].Execution_Cost_of_Node[i][j];
        return tot;
    }

    int end_time = Dag_Number[dag].Earliest_Start_Time[i][j] + Dag_Number[dag].Execution_Cost_of_Node[i][j];
    int k=0;
    while(k < processor_scheduler[j].size())
    {
        k=k+1;
        if (Dag_Number[dag].Earliest_Start_Time[i][j] < processor_scheduler[j][k].end_time && end_time > processor_scheduler[j][k].start_time) {
            end_time = processor_scheduler[j][k].end_time;
        }
    }
    return end_time;
}



void HEFT_Scheduling(int dagIndex) {
    processor_slot tempSlot;
    processor_slot tempSlot2;
    int maxLength = -1;
    int minEndTime;
    int minTime;
    minTime = INT_MAX;
    int taskIndex;


    taskIndex = Dag_Number[dagIndex].rank_task[0].second;
    DAG& current_dag = Dag_Number[dagIndex];

    int i=0;
    while(i<Dag_Number[dagIndex].proc)
    {

       current_dag.Earliest_Start_Time[current_dag.rank_task[0].second][i] = 0;
        if(current_dag.Execution_Cost_of_Node[taskIndex][i] < minTime) {
            minTime = current_dag.Execution_Cost_of_Node[taskIndex][i];
            current_dag.Processor_Taking_Minimum_Time[taskIndex] = i;
        }
        i=i+1;
    }

    tempSlot.task_no = taskIndex;
    tempSlot.d = dagIndex;
    tempSlot.start_time = 0;
    tempSlot.end_time = minTime;
    current_dag.Actual_Finish_Time[current_dag.rank_task[0].second] = minTime;

    int currentIndex;

    int h=0;
    while(h < Dag_Number[dagIndex].tasks)
    {
        currentIndex = Dag_Number[dagIndex].rank_task[h].second;

        int j=0;
        while(j< Dag_Number[dagIndex].proc)
        {
            Dag_Number[dagIndex].Earliest_Start_Time[currentIndex][j] = Calculation_Time_Estimated(currentIndex, j, dagIndex);
            j=j+1;
        }
        minEndTime = INT_MAX;
        int processorIndex = 0;


        j=0;
        while(j<Dag_Number[dagIndex].proc)
        {
            DAG& current_dag = Dag_Number[dagIndex];
            current_dag.Early_Finish_Time[currentIndex][j] = Early_Finish_Time_cal(currentIndex, j, dagIndex);
            if(current_dag.Early_Finish_Time[currentIndex][j] < minEndTime) {
                minEndTime = current_dag.Early_Finish_Time[currentIndex][j];
                processorIndex = j;
            }

            j=j+1;
        }

        DAG& current_dag = Dag_Number[dagIndex];
        int dagIndex2=dagIndex;
        current_dag.Processor_Taking_Minimum_Time[currentIndex] = processorIndex;
        tempSlot2.start_time = minEndTime - current_dag.Execution_Cost_of_Node[currentIndex][processorIndex];
        int currentIndex2=currentIndex;
        tempSlot2.end_time = minEndTime;

        current_dag.Actual_Finish_Time[currentIndex] = minEndTime;
        tempSlot2.d = dagIndex2;
        tempSlot2.task_no = currentIndex2;
        maxLength = max(minEndTime, maxLength);


        processor_scheduler[processorIndex].push_back(tempSlot2);

       h=h+1;
    }
}



void FairnessPolicy_finishtime()
{
	cout<<endl<<"Fairness policy output:"<<endl;
	int DAG_Selection;
    int Node_Selection;
    int Processor_selection;
    int Counter_bit=1;
    int Combined_Finish_Time=0;
    int Itself_Finish_Time=0;
    int task[2];
	vector<int> v[2]; int n=1;
	double Slowed_downed;
	vector <int> dag;

	int i=0;
	while(i<=1)
    {
        DAG& current_dag = Dag_Number[i];
        Slow_Down.push_back(make_pair(0.0,i));
		Make_Span.push_back(make_pair(i, current_dag.Actual_Finish_Time[(current_dag.tasks)-1]));
		Ready_Queue[i].push_back(make_pair(current_dag.Actual_Finish_Time[0],0));
		dag.push_back(1);
		task[i]=0;
		i=i+1;
    }
    DAG_Selection = (Make_Span[0].second < Make_Span[1].second) ? Make_Span[1].first : Make_Span[0].first;

	int c=0;
	int Task_Local_Itself;
	double Finish_Time_Local_Itself;
for (;;) {
  if (!dag[0] && !dag[1]) {
    break;
  }

  {
		Counter_bit=0;
		Node_Selection = Ready_Queue[DAG_Selection][0].second;
		Processor_selection = Dag_Number[DAG_Selection].Processor_Taking_Minimum_Time[Node_Selection];
        if (!Ready_Queue[DAG_Selection].empty()) {
  Ready_Queue[DAG_Selection].pop_back();
  std::reverse(Ready_Queue[DAG_Selection].begin(), Ready_Queue[DAG_Selection].end());
}
		if(Dag_Number[DAG_Selection].tasks <= task[DAG_Selection])
        {
            dag[DAG_Selection]=0;
            DAG_Selection = (dag[DAG_Selection] = 0, (DAG_Selection != 0) ? (dag[DAG_Selection-1] != 0 ? 0 : DAG_Selection) : (dag[DAG_Selection+1] != 0 ? 1 : 0));
        }
        else
           	{
            DAG& selection_of_dag = Dag_Number[DAG_Selection];
			for(int a= 0; a < selection_of_dag.child[Node_Selection].size() ; a=a+1)
			{
				Task_Local_Itself = selection_of_dag.child[Node_Selection][a];
				Finish_Time_Local_Itself =  selection_of_dag.Actual_Finish_Time[Task_Local_Itself];


				for(int m =0 ; m < v[DAG_Selection].size();m++)
				{
                    n = (v[DAG_Selection][m] != Task_Local_Itself) ? n : 0;
				}
				if(n)
				{
                    v[DAG_Selection].emplace_back(Task_Local_Itself);
                    Ready_Queue[DAG_Selection].emplace_back(Finish_Time_Local_Itself, Task_Local_Itself);

				}
				n=1;
			}
			sort(Ready_Queue[DAG_Selection].begin(), Ready_Queue[DAG_Selection].end());

			Itself_Finish_Time = selection_of_dag.Actual_Finish_Time[Node_Selection];
			Combined_Finish_Time = selection_of_dag.Execution_Cost_of_Node[Node_Selection][Processor_selection]+Completion_TIme;

            Completion_TIme = Combined_Finish_Time;
            Slowed_downed = static_cast<double>(Itself_Finish_Time) / static_cast<double>(Combined_Finish_Time);
            std::cout << "Graph_Number " << DAG_Selection << " Task_NUmber " << Node_Selection << " Finish Time " << Completion_TIme << '\n';
            Slow_Down.erase(Slow_Down.begin());
            Slow_Down.emplace_back(Slowed_downed, DAG_Selection);
            std::sort(Slow_Down.begin(), Slow_Down.end());
            ++task[DAG_Selection];
            DAG_Selection = std::rand() % 2;

	    	}
	}
}
}
void print()
{
    int x;
    x=0;
    while(x<2)
    {
        cout <<"Directed Acylic Graph "<<x<<" Matrix"<<endl;
        int y=0;
        while(y<Dag_Number[x].tasks)
        {
         int z=0;
            while(z < Dag_Number[x].tasks)
            {
                cout<< Dag_Number[x].graph_matrix[y][z] <<" ";
                z++;
            }
            cout<<endl;
           y++;
        }
        x++;
    }

    x=0;
    while(x<2)
    {
       cout <<"Parents for Each Task "<<x<<endl;
      int y=0;
        while(y<Dag_Number[x].tasks)
        {
         int z=0;
            while(z < Dag_Number[x].Parent_Number[y].size())
            {
                cout<< Dag_Number[x].Parent_Number[y][z] <<" ";
                z++;
            }
            cout<<endl;
           y++;
        }
        x++;
    }

    x=0;
    while(x<2)
    {
      cout <<"Execution Cost of Tasks(Nodes) with Respect to each Processor "<<x<<endl;
      int y=0;
      while(y<Dag_Number[x].tasks)
      {
         int z=0;
            while(z < Dag_Number[x].proc)
            {
                cout<< Dag_Number[x].Execution_Cost_of_Node[y][z] <<" ";
                z++;
            }
            cout<<endl;
           y++;
        }
        x++;
    }

    x=0;
    while(x<2)
    {
      cout <<"Rank of Each Task "<<x<<endl;
      int y=0;
      while(y<Dag_Number[x].tasks)
      {
         DAG& current_dag = Dag_Number[x];

         cout<< current_dag.rank_task[y].second <<"-->"<<current_dag.rank_task[y].first ;

           cout<<endl;
           y++;
        }
        x++;
    }


}
int main()
{
    int Counter_DAG=0;
	int edges_count;

    while(Counter_DAG<2)
    {
        memset(Dag_Number[Counter_DAG].graph_matrix, -1, sizeof(Dag_Number[Counter_DAG].graph_matrix));
        Counter_DAG=Counter_DAG+1;
    }

    Counter_DAG=0;
    while(Counter_DAG<Number_of_Graphs)
    {
        cout << "Enter number of tasks and processors for DAG "<<Counter_DAG <<endl;;

    	cin >> Dag_Number[Counter_DAG].tasks;
        cin >> Dag_Number[Counter_DAG].proc;
    	Max_Number_of_Processors = Dag_Number[Counter_DAG].proc;


        Counter_DAG=Counter_DAG+1;
    }

	srand(time(0));
int x=0;
while(x<Number_of_Graphs)
{
    int i=0;
	while(i < Dag_Number[x].tasks)
    {
        double temp=0;
        int j=0;
        while(j < Dag_Number[x].proc)
    	{
            DAG& current_dag = Dag_Number[x];
			current_dag.Execution_Cost_of_Node[i][j] = rand()%15;
			if(current_dag.Execution_Cost_of_Node[i][j]==0)
            {
					current_dag.Execution_Cost_of_Node[i][j]=current_dag.Execution_Cost_of_Node[i][j]+1;
            }

            temp = temp + current_dag.Execution_Cost_of_Node[i][j];
            j=j+1;
		}
			Dag_Number[x].Average[i] = (double) temp / (double) Dag_Number[x].proc;
        i=i+1;
	}
    x=x+1;
}
	Edge_Additions();
    int s=0;
    while(s<Number_of_Graphs)
	{
        int t=0;
		while(t < Dag_Number[s].tasks)
	 	{
        	Rank_Calculation(t,s);
            t=t+1;
    	}
        s=s+1;
	}
    s=0;
    while(s<Number_of_Graphs)
	{
        int t=0;
		while(t < Dag_Number[s].tasks)
	 	{
            Dag_Number[s].rank_task.push_back(make_pair(Dag_Number[s].Rank_of_node[t], t));
            t=t+1;
    	}
        s=s+1;
	}
      s=0;
    while(s<Number_of_Graphs)
	{
        sort(Dag_Number[s].rank_task.rbegin(), Dag_Number[s].rank_task.rend());
        s=s+1;
    }

    s=0;
    while(s<Number_of_Graphs)
	{
        int t=0;
		while(t < Dag_Number[s].tasks)
	 	{
			HEFT_Scheduling(s);
            t=t+1;
    	}
        s=s+1;
	}
	cout<<endl<<endl;

	print();

	FairnessPolicy_finishtime();

	Completion_TIme = 0;



    return 0;
}
