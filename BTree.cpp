// Vatsal Soni 
// 2018201005
#include <bits/stdc++.h>
using namespace std;
#define ll long long

ll order;                  // order of b+ tree
ll BlockSize,m,f;
ll i,j;
string l,str;
string f_name;                      // file name
vector<string> output;              // output buffer

string get_type_of_query(string s);
void write_file(vector<string> &v);
ll convert_to_int(string s);

// Node structure
struct node
{
    ll count,leaf;
    ll *duplicate;
    ll *data;
    node **child;
    node *ngh;
};

struct tree 				// structure of tree
{
    public:
    
        ll item;
        node *root;

        tree()
        {
            root = NULL;
        }

        void setdata(ll data)
        {
            item=data;
        }

        node* getroot()
        {
            return root;
        }

        node* create_node(ll l);
        void split_node(node *parent,node *temp);
        void query_process(vector<string> &out,string l);
        void duplicate_inc(node *temp);
        void insert(node *parent , node *temp);
        void insert_leaf(node *temp,ll item);
        ll range_query(ll low,ll high);
      
};tree t;

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);

    // if parameters are not valid
    if(argc != 4)          
    {
        cout<<"Input is not valid";
        return 0;
    }   

    f_name = argv[1];

    m = convert_to_int(argv[2]);

    BlockSize = convert_to_int(argv[3]);

    ifstream infile(f_name);
    
    double bz = (double)BlockSize + 4;

    order=ceil(bz/12); 

    int flag = 1;
    int i;
    
    vector<vector<string>> input(m-1,vector<string>(BlockSize));
    // while there is unread records in input file , run the loop
    while(flag)                 
    {    
        int j;
        for(i = 0 ; i < m - 1 ; i++)
        {
            j = 0;
            while(j < BlockSize)
            {
                string x;
                if(!getline(infile,x))
                {   
                    flag = 0;  
                    break;
                }   
                else
                {
                    input[i][j] = x;
                    // string query = get_type_of_query(l);
                }   
                j++;
            }
            if(!flag)
                break;  
        }

        ll y;
        
        for ( y = 0; y <= i-1; y++)
        {
            ll z;
            for (z = 0; z<input[y].size(); z+=1)
            {
                t.query_process(output,input[y][z]);
                if(output.size()>BlockSize)write_file(output);
            }
        }
        if(i<=m-2)
        {
            ll z;
            for(z=0;z<input[i].size();++z)
            {
                t.query_process(output,input[i][z]);
                if(output.size()>BlockSize)write_file(output);   
            }
        }
    }
    
    write_file(output);
    
    return 0;
}

ll convert_to_int(string s)
{
    ll x;
    stringstream ss(s);
    ss>>x;
    return x;
}

string get_type_of_query(string s)
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

void write_file(vector<string> &v)
{
    ll i;
    //cout<<"in write_file\n";
    ofstream outfile("output.txt", std::ios_base::app);
    ll sw=v.size();
    //cout<<sw<<endl;
    for(i=0;i<sw;++i)
    { 
        string s = get_type_of_query(l);
        outfile<<v[i]<<endl;
        cout<<v[i]<<endl;
    }    
    v.clear();  
    outfile.close();
}

node* tree::create_node(ll l)
{

    node *ne;
    ne = new node;
    ll no_data = order+1;
    ll no_child = order+2;
    ne->data = new ll[no_data];
    ne->child = new node*[no_child];
    ne->duplicate = new ll[no_data];
    ne->count=0;                    // count number of keys
    ne->leaf=l;
    ne->ngh=NULL;                       
    ll i=0;

    while(i<=order)
    {
        ne->duplicate[i] = 1;
        ne->child[i] = NULL;    
        i++;
    }    
    ne->child[i] = NULL;        
    return ne;
}

void tree::duplicate_inc(node *temp)
{
    ll i=0;
    ll fg = 0;
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
                fg = 1;
                break;
            }
        }
        else
        {
            i++;
        }
    }
}

void tree::split_node(node *parent,node *temp)
{
    ll flag=1;
    ll leaf=0;
    
    if(temp==root)
    {
        root=create_node(leaf);
        root->data[0]=temp->data[order/2];
        parent=root;
        root->count=root->count+1;
        parent->child[0]=temp;

        flag=0;
    }
    
    for(int y=0;y<parent->count+1;y++)
    {
        if(parent->child[y]==temp)
        {
            ll i=parent->count+1;

            while(i>y+1)
            {
                 ll ab = i - 1;   
                 parent->child[i]=parent->child[ab];
                 i--;
            }

            parent->child[y+1] = create_node(leaf);

            if(temp->child[0] == NULL)
                leaf = 1;
            else 
                leaf = 0;
            
            
            ll z;
            long long limit1 = order/2;
            for(z = 0;z <= limit1;z++)
                (parent -> child[y + 1])->child[z] = temp->child[z + 1 + limit1];
            
            ll zt = order/2;
            zt++;
            
            ll pmax = zt-1;
            temp->count=pmax;
            pmax=order-(order/2+1-leaf);

            for(z=zt;z<=order;z++)
                temp->child[z] = NULL;

            (parent->child[y+1])->count=pmax;
            
            ll k=0;
            zt=order/2+1-leaf;
            for (z = zt;z<=order; z=z+1)
            {
                (parent->child[y+1])->duplicate[k]=temp->duplicate[z];
                (parent->child[y+1])->data[k]=temp->data[z];
                k+=1;
            }
            if (leaf == 0)
            {
                //do nothing
            }
            else
            {
                (parent->child[y+1])->ngh=temp->ngh;
                ll p;
                temp->ngh=parent->child[y+1];
            }

            if(flag==1)
            {
                insert_leaf(parent,temp->data[order/2]);
            }
            break;
        }
    }
}

ll tree::range_query(ll low,ll high)
{
    ll i = 0;
    node *temp = root;
    ll count = 0;
    
    while(temp)
    {
        if(temp->child[0] == NULL && temp->data[i]>=low)
        {
            
            ll j = i;
            
            while(j <= temp -> count - 1 && temp -> data[j] < high+1)
            {    
                ll rep = temp->duplicate[j];
                count=count + rep;
                j++;
            }    
            temp=temp->ngh;
            ll p;
            ll fg = 0;
            while(temp)
            {
                for(p=0;p < temp->count;p++)
                {
                    if(temp->data[p] > high)
                    {
                        fg = 1;
                        break;
                    }
                    string s1 = get_type_of_query(l);    
                    count+=temp->duplicate[p];
                }
                if(fg == 1)
                    break;
                temp=temp->ngh;
            }
            return count;
        }
        else if(low < temp->data[i] || ((i + 1) == (temp->count + 1)))
        {
            temp=temp->child[i];
            i=0;
        }
        else
            i++;
    }
    return 0;
}

string getString(ll ans)
{
    if(ans<=0)
        return "NO";
    else
        return "YES";
}

void tree::query_process(vector<string> &out,string l)
{
   
    string query = get_type_of_query(l);
    //cout<<query<<endl;
    
    if(query == "RANGE")
    {   
        ll j;
        ll low;
        ll high;
        string temp = l.substr(6);

        j=0;

        while(temp[j]!=' ')
        {
            j = j + 1;
        }
    
        string shigh = temp.substr(j+1);
        string slow = temp.substr(0,j);

        high = stoi(shigh);
        low = stoi(slow);
        
        ll ans = t.range_query(low,high);
        string str_ans = to_string(ans);
        out.push_back(str_ans);            
    }

    else if(query == "INSERT")    
    {
        ll sti=stoi(l.substr(7));
        t.setdata(sti);
        t.insert(t.getroot(),t.getroot());
    }

    else if(query == "COUNT")
    {
        t.setdata(stoi(l.substr(6)));

        ll i=0;
        ll ans;
        
        node *temp=root;
        
        while(temp != NULL)
        {
            if(!(temp->child[0]) && temp->data[i]==item)
            {
                ans = temp->duplicate[i];
                break;
            }
            else if((item<=temp->data[i]-1)||i==temp->count)
            {
                temp=temp->child[i];
                i = 0;
            }
            else 
                i = i + 1;
        }
        if(temp == NULL)
            ans = 0;

        out.push_back(to_string(ans));
    }

    else if(query == "FIND")
    {
        t.setdata(stoi(l.substr(5)));
        
        ll i=0;
        ll ans;
        node *temp=root;

        while(temp!=NULL)
        {
            if(!(temp->child[0]) && temp->data[i]==item)
            {
                ans = temp->duplicate[i];
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
            ans = 0;

        string s1 = getString(ans);

        out.push_back(s1);
    }
   
}

void tree::insert(node *parent , node *temp)
{
    // If tree is empty
    if(!temp)   
    {
        int leaf = 1;
        temp = create_node(leaf);
        temp->duplicate[0]=1;
        temp->count=temp->count+1;
        temp->data[0]=item;
        root = temp;
    }
    // If node is leaf node
    else if(temp->child[0] == NULL)
    {
        ll i=0;
        ll fg = 0;
        while(i < temp->count)
        {
            // If repeat then duplicate_inc duplicate value
            if(temp->data[i] == item)
            {
                duplicate_inc(temp);
                fg = 1;
                break;
            }
            i++;
        }
        // Otherwise insert data into leaf node
        if(fg == 0)
        {
            insert_leaf(temp,item);
        }
    }
    else if(item>=temp->data[temp->count-1])
    {
        insert(temp,temp->child[temp->count]);
    }
    else
    {
        ll i;
        for(i=0;i<=temp->count-1;i=i+1)
        {
            if(order > i)
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
    if(order-1 < temp->count)
    {
        split_node(parent,temp);
    }
}

void tree::insert_leaf(node *temp,ll item)
{
    if(item>temp->data[temp->count-1])
    {
        temp->data[temp->count]=item;
    }

    else
    {
        ll i;
        for(i=0;i < temp->count;i++)
        {
            if(item<=temp->data[i]-1)
            {
                ll j;
                for(j=temp->count;j>i;j--)
                {
                    temp->duplicate[j]=temp->duplicate[j-1];
                    temp->data[j]=temp->data[j-1];
                }
                
                temp->duplicate[i]=j;
                temp->data[i]=item;

                break;
            }
        }
    }
    temp->count=temp->count+1;
}