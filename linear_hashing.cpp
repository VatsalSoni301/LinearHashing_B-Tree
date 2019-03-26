// Vatsal Soni
// 2018201005

#include <bits/stdc++.h>
using namespace std;
#define ull long long

ull convert_string_to_int(string s);
ull rehash();
ull hashing(ull input);
ull split();
ull linear_hashing(ull input, ull size);
ull write_output(vector<ull> v);

ull m , b;     		
ull split_ptr,split_val;				
ull hash_value = 2;
ull number_of_records = 0;
char* f_name;
ull bucket_size;
ifstream file_name;
vector<set<ull> > v; 		
vector<ull> output; 

int main(int argc , char** argv)
{
	if(argc != 4)
	{
		cout<<"Input invalid";
		return 1;
	}	

	f_name = argv[1];
	m = convert_string_to_int(argv[2]);
	b = convert_string_to_int(argv[3]);

	ifstream infile(f_name);
	
	cout<<"number of buffers: "<<m<<endl;
	cout<<"\nbuffer size: "<<b<<endl;
	bucket_size = (b/4);
	set <ull> t0,t1;
	v.push_back(t0);
	v.push_back(t1);
	split_ptr = 0;
	split_val = 2;
	
	ull flag = 1,i;
	vector < vector <ull> > input(m - 1, vector<ull> (b) );
	ull j;
	cout<<"\n\nOUTPUT \n";
	while(flag)					
	{   		
		for(i = 0 ; i < m - 1 ; i++)
		{
			j=0;
			while(j < b)
			{
				ull x;
				if(infile >> x)
				{	
					input[i][j] = x;
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
		
		ull p=0;

		for( p=0;p<i;p++)
		{
			for(ull q=0;q<b;q++)
			{
				hashing(input[p][q]);
			}	
		}
		if(i<=m-2)
		{
			ull q=0;
			while(q<j)
			{
				hashing(input[p][q]);
				q++;
			}
		}	 		
	}

	write_output(output);
	cout<<"\n\nOutput is stored in output.txt\n";
	return 0;
}

ull linear_hashing(ull input, ull size)
{
	ull x;
	x = input % hash_value;
	// cout<<"x="<<x<<" ";
	if(input < 0)
	{
		x+=hash_value;
	}	

	while(x > size)
	{
		ull temp = x;
		ull bt = -1;

		while(temp)
		{
			bt+=1;
			temp/=2;
		}	
		x^=1<<bt;
	}	
	// cout<<"x="<<x;
	// cout<<endl;
	return x;
}

ull convert_string_to_int(string s)
{
	ull x;
	stringstream ss(s);
	ss>>x;
	return x;
}

ull rehash()
{
	vector<ull> temp;
	set <ull > ::iterator it = v[split_ptr].begin();
	while(it!=v[split_ptr].end())
	{
		ull index = linear_hashing(*it,v.size());
		if(split_ptr != index)
		{
			temp.push_back(*it);
			v[split_ptr].erase(*it);
			//v[index].insert(*it);
		}	
		it++;
	}	
	ull i = 0;
	while(i<temp.size())
	{
		ull index = linear_hashing(temp[i],v.size());
		v[index].insert(temp[i]);
		i++;
	}	
	return 0;
}

ull split()
{
	bucket_size += (b/4);
	
	set<ull> s;
	v.push_back(s);
	hash_value = 1<<(ull)(ceil(log2(v.size())));

	rehash();
	
	split_ptr = (split_ptr + 1) % split_val;
	if(!split_ptr)
		split_val*=2;

	return 0;
}

ull hashing(ull input)
{
	ull number_of_buckets = v.size() - 1;
	ull index;
	index = linear_hashing(input,number_of_buckets);
	ull size_of_bucket;
	size_of_bucket = v[index].size();
	v[index].insert(input);
	if(v[index].size() > size_of_bucket)
	{
		number_of_records++;
		output.push_back(input);
		if(output.size() != b)
		{
			// do nothing
		}
		else
		{
			write_output(output);
			output.clear();
		}
	} 

	double bz = (double)bucket_size;
	double ratio = number_of_records/bz;
	if(ratio > 0.75)
	{
		split();
	}
	return 0;	
}

ull write_output(vector<ull> v)
{
	ofstream outfile("output.txt",ios_base::app);
	ull i =0;
	while(i<output.size())
	{
		outfile<<output[i]<<endl;
		cout<<output[i]<<endl;
		i++;
	}	
	outfile.close();
	return 0;
}