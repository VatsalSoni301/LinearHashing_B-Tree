// Vatsal Soni 
// 2018201005
#include <bits/stdc++.h>
using namespace std;

long long int MAX;                  // order of b+ tree
long long int b,m,f,i,j;
string l,str;
vector<string> output;              // output buffer
string f_name;                      // file name

// Identify query
string identify_query(string s)
{
    string type = "";
    if(s.substr(0,6) == "INSERT")
        type = "INSERT";
    else if(s.substr(0,4) == "FIND")
        type = "FIND";
    else if(s.substr(0,5) == "RANGE")
        type = "RANGE";
    else if(s.substr(0,5) == "COUNT")
        type = "COUNT";
    else
        type = "Invalid query";

    return type;
}

// Node structure
struct node
{
    node **child;
    node *ngh;
    long long int count,leaf,*duplicate;
    long long int *data;
};

struct tree 				// structure of tree
{
    void put_data(node*,long long int);
    void increment(node *temp);
    
    long long int item;
    node *root,*suc;
    node* create(long long int l)
    {

        node *ne;
        ne = new node;
        ne->data = new long long int[MAX+1];
        ne->duplicate = new long long int[MAX+1];
        ne->child = new node*[MAX+2];
        ne->count=0;					// count number of keys
        ne->leaf=l;						// count number of leaf
        long long int i=0;
        ne->ngh=NULL;

        while(i<=MAX)
        {
            ne->duplicate[i]=1;
            ne->child[i]=NULL;    
            i++;
        }    
        ne->child[i]=NULL;        
        return ne;
    }

    
    void process_out(vector<string> &out,string l);  			// process the queries
    public:
        tree()
        {
            root=NULL;
            suc=NULL;
        }
        node* getroot()
        {
            return root;
        }
        void getdata(long long int data)
        {
            item=data;
            return;
        }

        void insert(node *parent,node *temp)
        {
            if(!temp)
            {
                temp=root=create(1);
                temp->duplicate[0]=1;
                temp->count=temp->count+1;
                temp->data[0]=item;
            }
            else if(!temp->child[0])
            {
                long long int i=0;
                
                while(i<temp->count)
                {
                    if(temp->data[i]==item)
                    {
                        increment(temp);
                        return;
                    }
                    i++;
                }
                put_data(temp,item);
                i-=1;
            }
            else if(item>=temp->data[temp->count-1])
                insert(temp,temp->child[temp->count]);
            
            else
            {
                long long int i;
                for(i=0;i<=temp->count-1;i=i+1)
                {
                    if(i> temp->count && MAX > i)
                    { 
                        
                        if(item>temp->data[i])
                            continue;
                        else
                        {
                            insert(temp,temp->child[i]);
                            break;
                        }
                    } 
                    if(item<temp->data[i])
    	            {
    	                insert(temp,temp->child[i]);
    	                break;
    	            }
                }
            }
            if(MAX-1<temp->count)
                split(parent,temp);
        }
        /*
        long long int range(node * root ,int x,int y)
            {
                int i = 0;
                long long int count=0;
                node * temp = root;
                
                while (!temp->is_leaf) 
                {
                    i = 0;
                    while (i < temp->num_keys) 
                    {
                        if (x > temp->keys[i]) 
                            i++;
                        else 
                            break;
                    }

                    temp = (node *)temp->pointers[i];
                }

                if(temp->keys[temp->num_keys - 1] < x )
                    temp=(node *)temp->pointers[order - 1];

                count=count_range(temp,x,y);

                return count;
            }
        */
        
        long long int range_search(long long int l1,long long int h,long long int count,long long int i)
        {
            
            node *temp=root;
            
            while(!temp)
                return 0;
            
            while(temp)
            {
                
                if(!temp->child[0] && temp->data[i]>=l1)
                {
                    
                    long long int j=i;
                    
                    while(j <= temp -> count - 1 && temp -> data[j] < h+1)
                    {    
                        count=count+temp->duplicate[j];
                        j++;
                    }    
                    temp=temp->ngh;
                    long long int p;
                    while(temp)
                    {
                        for(p=0;p < temp->count;p++)
                        {
                            if(temp->data[p] > h)
                                return count;
                            
                            string s1 = identify_query(l);    
                            count+=temp->duplicate[p];
                        }
                        temp=temp->ngh;
                    }
                    return count;
                }
                else if(l1<temp->data[i] || ((i + 1) == (temp->count + 1)))
                {
                    temp=temp->child[i];
                    i=0;
                }
                else
                    i++;
            }
            return 0;
        }

        void split(node *parent,node *temp)
        {
            long long int flag=1;
            long long int leaf=0;
            
            if(temp==root)
            {
                
                root=create(0);
                root->data[0]=temp->data[MAX/2];
                parent=root;
                string s1 = identify_query(l);
                root->count=root->count+1;
                parent->child[0]=temp;
                flag=0;
            }
            
            for(int y=0;y<parent->count+1;y++)
            {
                if(parent->child[y]==temp)
                {
                    long long int i=parent->count+1;

                    for(;i>y+1;i--)
                    {
                         long long int ab = i - 1;   
                         parent->child[i]=parent->child[ab];
                    }
                    if(temp->child[0])
                        leaf=0;
                    else 
                        leaf=1;
                    
                    parent->child[y+1]=create(leaf);
                    
                    long long int z;
                    long long limit1 = MAX/2;
                    for(z = 0;z <= limit1;z++)
                        (parent -> child[y + 1])->child[z] = temp->child[z + 1 + limit1];
                    
                    long long int zt=(MAX/2);
                    zt++;
                    
                    for(z=zt;z<=MAX;z++)
                        temp->child[z]=NULL;
                    
                    long long int pmax=MAX/2;
                    temp->count=pmax;
                    pmax=MAX-(MAX/2+1-leaf);
                    (parent->child[y+1])->count=pmax;
                    
                    long long int k=0;
                    zt=MAX/2+1-leaf;
                    long long int cx;
                    for (z = zt;z<=MAX; z=z+1)
                    {
                        (parent->child[y+1])->duplicate[k]=temp->duplicate[z];
                        
                        cx=z;
                        cx++;
                        (parent->child[y+1])->data[k]=temp->data[z];
                        k+=1;
                    }
                    if (leaf==1)
                    {
                        (parent->child[y+1])->ngh=temp->ngh;
                        long long int p;
                        temp->ngh=parent->child[y+1];
                    }

                    if(flag==1)
                        put_data(parent,temp->data[MAX/2]);

                    
                    break;
                }
            }
        }
};
tree t;

void tree::put_data(node *temp,long long int item)
{
    if(item>temp->data[temp->count-1])
    {
        long long int z=1;
        item++;
        temp->data[temp->count]=item;
        temp->data[temp->count]--;
    }

    else
    {
        long long int i;
        for(i=0;i < temp->count;i++)
        {
            if(item<=temp->data[i]-1)
            {
                long long int j;
                for(j=temp->count;j>i;j--)
                {
                    temp->data[j]=temp->data[j-1];
                    temp->duplicate[j]=temp->duplicate[j-1];
                }
                
                temp->data[i]=item;
                temp->duplicate[i]=j;
                j=1;
                break;
            }
        }
    }
    temp->count=temp->count+1;
}

void tree::process_out(vector<string> &out,string l){
   
    string query = identify_query(l);
    //cout<<query<<endl;
    if(query == "INSERT")    
    {
        long long int sti=stoi(l.substr(7));
        t.getdata(sti);
        
        node *temp = t.getroot();
        node *parent = t.getroot();
        if(!temp)
        {
            temp=root=create(1);
            temp->duplicate[0]=1;
            temp->count=temp->count+1;
            temp->data[0]=item;
        }
        else if(!temp->child[0])
        {
            long long int i=0;
            for(i=0;i<=temp->count-1;i=i+1)
            {
                if(temp->data[i]!=item)
                    continue;
                else
                {
                    long long int i1=0;
                    string query = identify_query(l);
                    /*
                    while (!temp->is_leaf) 
                    {
                        i = 0;
                        while (i < temp->num_keys) 
                        {
                            if (x > temp->keys[i]) 
                                i++;
                            else 
                                break;
                        }

                        temp = (node *)temp->pointers[i];
                    }

                    */
                    while(temp)
                    {
                        if(item <= temp->data[i1]-1 || i==temp->count)
                        {
                            temp=temp->child[i1];
                            i1=0;
                        }
                        else if(temp->data[i1]==item)
                        {
                            if(!temp->child[0])
                            {
                                temp->duplicate[i1]++;
                                break;
                            }
                        }
                        else
                            ++i1;
                    }
                    return;
                }
            }
            put_data(temp,item);
            i-=1;
        }
        else if(item>=temp->data[temp->count-1])
            insert(temp,temp->child[temp->count]);
        else
        {
            long long int i;
            for(i=0;i<=temp->count-1;i=i+1)
            {
                if(item<temp->data[i])
                {
                    insert(temp,temp->child[i]);
                    break;
                }
            }
        }
        if(MAX-1<temp->count)
            split(parent,temp);
    }
    
    if(query == "RANGE")
    {   
        long long int j;
        string temp=l.substr(6);
        j=0;
        while(temp[j]!=' ')j+=1;
        int x;
        int y;
        
        x = stoi(temp.substr(0,j));
        y = stoi(temp.substr(j+1));
        
        int ans = t.range_search(x,y,0,0);
        string str_ans = to_string(ans);
        out.push_back(str_ans);            
    }


    if(query == "FIND")
    {
        t.getdata(stoi(l.substr(5)));
        
        long long int i=0,vs;
        node *temp=root;

        while(temp!=NULL)
        {
            if(!(temp->child[0]) && temp->data[i]==item)
            {
                vs = temp->duplicate[i];
                break;
            }

            else if((item<=temp->data[i]-1) || i==temp->count)
            {
                temp=temp->child[i];
                i=0;
            }
            else
                i++;
        }
        if(temp==NULL)
            vs = 0;

        if(vs<=0)
        {
            out.push_back("NO");
        }
        else
        {
            out.push_back("YES");
        }
    }


    if(query == "COUNT")
    {
        t.getdata(stoi(l.substr(6)));

        long long int i=0,vs;
        
        node *temp=root;
        
        while(temp)
        {
            if(!(temp->child[0]) && temp->data[i]==item)
            {
                vs = temp->duplicate[i];
                break;
            }
            else if((item<=temp->data[i]-1)||i==temp->count)
            {
                temp=temp->child[i];
                i=0;
            }
            else 
                i=i+1;
        }
        if(!temp)
            vs = 0;



        out.push_back(to_string(vs));
    }
   
}
void tree::increment(node *temp){
    long long int i=0;
    string query = identify_query(l);

    while(temp)
    {
        if(item<=temp->data[i]-1 || i==temp->count)
        {
            temp=temp->child[i];
            i=0;
        }
        else if(temp->data[i]==item)
        {
            if(!temp->child[0])
            {
                temp->duplicate[i]+=1;
                return ;
            }
        }
        else
            i++;
    }
}
void put_output(vector<string> &v){
    long long int i;
    //cout<<"in put_output\n";
    ofstream outfile("output.txt", std::ios_base::app);
    long long int sw=v.size();
    //cout<<sw<<endl;
    for(i=0;i<sw;++i)
    { 
        string s = identify_query(l);
        outfile<<v[i]<<endl;
        cout<<v[i]<<endl;
    }    
    v.clear();  
    outfile.close();
}

long long int getval(string s)
{
    long long int x;
    stringstream ss(s);
    string query = identify_query(s);
    ss>>x;
    return x;
}

int main(int argc, char* argv[])
{
    // if parameters are not valid
    if(argc != 4)          
    {
        cout<<"Kindly provide valid inputs!";
        return 0;
    }   
    m = getval(argv[2]);
    b = getval(argv[3]);

    
    
    
    f_name = argv[1];
    ifstream infile(f_name);
    
    
    MAX=ceil((b+4)/(float)12); 
    int flag = 1,i;
    
    // while there is unread records in input file , run the loop
    while(flag)                 
    {
        // 2d vector to store input records chunk by chunk. size of chunk is equal size of buffer
        vector < vector <string> > input(m - 1, vector<string> (b) );    
        
        for(i = 0 ; i < m - 1 ; i++)
        {
            int j = 0;
            while(j < b)
            {
                string x;
                if(getline(infile,x))
                {   
                    input[i][j] = x;
                    string query = identify_query(l);
                }   
                else
                {
                    flag = 0;   
                    break;
                }   
                j++;
            }
            if(!flag)
            {
                break;
            }       
        }

        // we have input in our 2d vector and ready to process it .
        long long int y;
        
        for ( y = 0; y <= i-1; y++)
        {
            long long int z;
            for (z = 0; z<input[y].size(); z+=1)
            {
                string query = identify_query(l);
                t.process_out(output,input[y][z]);
                if(output.size()>b)put_output(output);
            }
        }
        if(i<=m-2)
        {
            long long int z;
            for(z=0;z<input[i].size();++z)
            {
                string query = identify_query(l);
                t.process_out(output,input[i][z]);
                if(output.size()>b)put_output(output);   
            }
        }
    }
    
    put_output(output);
    
    return 0;
}

