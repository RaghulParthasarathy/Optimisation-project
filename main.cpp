#include <bits/stdc++.h>
using namespace std;
#define mp make_pair
#define pb push_back
#define all(x) x.begin(), x.end()
typedef long long ll;

void factor(int n, vector<int> &v)
{
    for (int i = 1; i <n; i++)
    {
        if (n % i == 0)
        {
            v.pb(i);
        }
    }
}

void computeprefix(vector<vector<int>> &pre,vector<vector<int>> &v)
{
    for (int i = 0; i <= 10001; i++)
    {
        for (int j = 0; j <= 10001; j++)
        {
            if (i * j == 0)
            {
                pre[i][j] = 0;
                continue;
            }
            pre[i][j] = pre[i - 1][j] + pre[i][j - 1] - pre[i - 1][j - 1] + v[i - 1][j - 1];
        }
    }
}

void computefactors(vector<int>&v,int x,int limit)
{
    for(int i=1;i<=limit;i++)
    {
        if(x%i==0)
            v.pb(i);
    }
}

void only_positives(int i,int side_along_x,int side_along_y,int divisons_along_y,vector<pair<int, vector<int>>>&s,vector<vector<int>>&pre)
{
    int start_rectangle_number =-1;
    int end_rectangle_number=0;
    int sum_along_axis=0;
    for (int j = 0; j < divisons_along_y; j++)
    {   
        int value_enclosed = 0;
        value_enclosed = pre[i * side_along_x + side_along_x + 1][j * side_along_y + side_along_y + 1] + pre[i * side_along_x + 1][j * side_along_y + 1] - pre[side_along_x * i + side_along_x + 1][side_along_y * j + 1] - pre[side_along_x * i + 1][side_along_y * j + side_along_y + 1];
        if(start_rectangle_number==-1 && value_enclosed>0)
        {
            start_rectangle_number=j;
            end_rectangle_number=j;
            sum_along_axis+=value_enclosed;
        }
        else if(start_rectangle_number!=-1 && value_enclosed>=0)
        {
            end_rectangle_number++;
            sum_along_axis+=value_enclosed;
        }
        else if(start_rectangle_number!=-1)
        {
            vector<int> data(3);
            data[0]=i;
            data[1]=start_rectangle_number;
            data[2]=end_rectangle_number;
            s.pb(mp(sum_along_axis,data));
            start_rectangle_number=-1;
            sum_along_axis=0;

        }
        if(j==divisons_along_y-1 && start_rectangle_number!=-1)
        {
            vector<int> data(3);
            data[0]=i;
            data[1]=start_rectangle_number;
            data[2]=end_rectangle_number;
            s.pb(mp(sum_along_axis,data));   
        }
    }
}
void kadanes(int i,int side_along_x,int side_along_y,int divisons_along_y,vector<pair<int, vector<int>>>&s,vector<vector<int>>&pre)
{
    int start=0;
    int end=0;
    int tempstart=0;
    ll max_sum=-1e18;
    ll cur_sum=0;
    for (int j = 0; j < divisons_along_y; j++)
    {   
        int value_enclosed = 0;
        value_enclosed = pre[i * side_along_x + side_along_x + 1][j * side_along_y + side_along_y + 1] + pre[i * side_along_x + 1][j * side_along_y + 1] - pre[side_along_x * i + side_along_x + 1][side_along_y * j + 1] - pre[side_along_x * i + 1][side_along_y * j + side_along_y + 1];
        cur_sum+=value_enclosed;
        if(cur_sum>max_sum)
        {
            max_sum=cur_sum;
            start=tempstart;
            end=j;
        }
        if(cur_sum<0)
        {
            cur_sum=0;
            tempstart=j+1;
        }
    }
    if(max_sum>0)
    {
        vector<int> data(3);
        data[0]=i;
        data[1]=start;
        data[2]=end;
        s.pb(mp(max_sum,data)); 
    }
}

ll max_value_using_rectangles(int rev,int mtd,int factor1,int factor2,int max_value_enclosed,int no_of_rectangles_chosen,pair<int,int>&best_case_divisions,vector< vector<int> >&best_case_rects,vector<vector<int>>&pre)
{
    vector<pair<int, vector<int>>> s;
    int divisons_along_x = factor1;
    int side_along_x = 10000 / divisons_along_x;
    int divisons_along_y = factor2;
    int side_along_y = 10000 / divisons_along_y;
    // cout<<"divisons_along_x"<<divisons_along_x<<" divisons_along_y"<<divisons_along_y;
    for (int i = 0; i < divisons_along_x; i++)
    {
        if(mtd==0)
            only_positives(i,side_along_x,side_along_y,divisons_along_y,s,pre);
        else
            only_positives(i,side_along_x,side_along_y,divisons_along_y,s,pre);
    }

    sort(all(s));
    reverse(all(s));
    ll sum = 0;
    
    for (int i = 0; i < min((int)no_of_rectangles_chosen,(int)s.size()); i++)
    {
        if(s[i].first>0)
            sum += s[i].first;
    }

    if(sum>=max_value_enclosed){
        if(rev)
        {
            best_case_divisions.first=factor2;
            best_case_divisions.second=factor1;
        }
        else
        {
            best_case_divisions.first=factor1;
            best_case_divisions.second=factor2;
        }
        best_case_rects.clear();
        for (int i = 0; i < min((int)no_of_rectangles_chosen,(int)s.size()); i++)
        {
            best_case_rects.pb(s[i].second);
        }
    }
    return sum;
}

vector<pair<pair<float,float>,pair<float,float>>> create_edges(int reverse,vector<vector<int>>best_case_rects,pair<int,int>best_case_divisions)
{
    vector<pair<pair<float,float>,pair<float,float>>>edges;
    sort(all(best_case_rects));
    pair<float,float> vertex1;
    pair<float,float> vertex2;
    best_case_divisions.first=10000/best_case_divisions.first;
    best_case_divisions.second=10000/best_case_divisions.second;

    for(int i=0;i<best_case_rects.size();i++)
    {
        if(i==0)
        {
            vertex1=mp((float)0,(float)(best_case_divisions.second)*(best_case_rects[i][0]+1)-0.5);
            vertex2=mp((float)(best_case_divisions.first*best_case_rects[i][1])+1,(float)(best_case_divisions.second)*(best_case_rects[i][0]+1)-0.5);

            if(reverse)
            {
                swap(vertex1.first,vertex1.second);
                swap(vertex2.first,vertex2.second);
            }

            edges.pb(mp(vertex1,vertex2));

            vertex1=vertex2;
            if(reverse)
                vertex2.first=(float)(best_case_divisions.second)*(best_case_rects[i][0])+1;
            else
                vertex2.second=(float)(best_case_divisions.second)*(best_case_rects[i][0])+1;

            edges.pb(mp(vertex1,vertex2));

            vertex1=vertex2;
            if(reverse)
                vertex2.second=(float)(best_case_divisions.first*(best_case_rects[i][2]+1));
            else
                vertex2.first=(float)(best_case_divisions.first*(best_case_rects[i][2]+1));

            edges.pb(mp(vertex1,vertex2));

            vertex1=vertex2;
            if(best_case_rects[i+1][0]==best_case_rects[i][0])
            {
                if(reverse)
                    vertex2.first=(float)(best_case_divisions.second)*(best_case_rects[i][0]+1)-0.5;
                else    
                    vertex2.second=(float)(best_case_divisions.second)*(best_case_rects[i][0]+1)-0.5;

                edges.pb(mp(vertex1,vertex2));

                vertex1=vertex2; 
            }
            else
            {
                if(reverse)
                    vertex2.first=(float)(best_case_divisions.second)*(best_case_rects[i][0]+1);
                else
                    vertex2.second=(float)(best_case_divisions.second)*(best_case_rects[i][0]+1);

                edges.pb(mp(vertex1,vertex2));

                vertex1=vertex2; 
                if(reverse)
                    vertex2.second=0.5;
                else
                    vertex2.first=0.5;

                edges.pb(mp(vertex1,vertex2));

                vertex1=vertex2; 
                if(reverse)
                    vertex2.first=(float)(best_case_divisions.second)*(best_case_rects[i+1][0]+1)-int(0.5);
                else
                    vertex2.second=(float)(best_case_divisions.second)*(best_case_rects[i+1][0]+1)-int(0.5);

                edges.pb(mp(vertex1,vertex2));

                vertex1=vertex2; 
            }
        }
        else if (i != best_case_rects.size() - 1) 
        {
            if (reverse) 
                vertex2.second = (float)(best_case_divisions.first * best_case_rects[i][1] + 1) + 1;
            else 
                vertex2.first = (float)(best_case_divisions.first * best_case_rects[i][1] + 1) + 1;
            
            edges.pb(mp(vertex1, vertex2));
            vertex1 = vertex2;

            if (reverse)
                vertex2.first = (float)(best_case_divisions.second) * (best_case_rects[i][0]) + 1;
            else
                vertex2.second = (float)(best_case_divisions.second) * (best_case_rects[i][0]) + 1;
            
            edges.pb(mp(vertex1, vertex2));
            vertex1 = vertex2;

            if (reverse)
                vertex2.second = (float)(best_case_divisions.first * (best_case_rects[i][2] + 1) + 1) - (int)1;
            else
                vertex2.first = (float)(best_case_divisions.first * (best_case_rects[i][2] + 1) + 1) - (int)1;
            
            edges.pb(mp(vertex1, vertex2));
            vertex1 = vertex2;

            if (best_case_rects[i + 1][0] == best_case_rects[i][0]) 
            {
                if (reverse)
                    vertex2.first = (float)(best_case_divisions.second) * (best_case_rects[i][0] + 1) - 0.5;
                else
                    vertex2.second = (float)(best_case_divisions.second) * (best_case_rects[i][0] + 1) - 0.5;
                
                edges.pb(mp(vertex1, vertex2));
                vertex1 = vertex2;
            } 
            else 
            {
                if (reverse)
                    vertex2.first = (float)(best_case_divisions.second) * (best_case_rects[i][0] + 1);
                else
                    vertex2.second = (float)(best_case_divisions.second) * (best_case_rects[i][0] + 1);
                
                edges.pb(mp(vertex1, vertex2));
                vertex1 = vertex2;

                if (reverse)
                    vertex2.second = 0.5;
                else
                    vertex2.first = 0.5;
                
                edges.pb(mp(vertex1, vertex2));
                vertex1 = vertex2;

                if (reverse)
                    vertex2.first = (float)(best_case_divisions.second) * (best_case_rects[i + 1][0] + 1) - (int)0.5;
                else
                    vertex2.second = (float)(best_case_divisions.second) * (best_case_rects[i + 1][0] + 1) - (int)0.5;
                
                edges.pb(mp(vertex1, vertex2));
                vertex1 = vertex2;
            }
        } 
        else 
        {
            if (reverse)
                vertex2.second = (float)(best_case_divisions.first * best_case_rects[i][1]) + 1;
            else
                vertex2.first = (float)(best_case_divisions.first * best_case_rects[i][1]) + 1;
            
            edges.pb(mp(vertex1, vertex2));
            vertex1 = vertex2;

            if (reverse)
                vertex2.first = (float)(best_case_divisions.second) * (best_case_rects[i][0]) + 1;
            else
                vertex2.second = (float)(best_case_divisions.second) * (best_case_rects[i][0]) + 1;
            
            edges.pb(mp(vertex1, vertex2));
            vertex1 = vertex2;

            if (reverse)
                vertex2.second = (float)(best_case_divisions.first * (best_case_rects[i][2] + 1));
            else
                vertex2.first = (float)(best_case_divisions.first * (best_case_rects[i][2] + 1));
            
            edges.pb(mp(vertex1, vertex2));
            vertex1 = vertex2;

            if (reverse)
                vertex2.first = (float)(best_case_divisions.second) * (best_case_rects[i][0] + 1);
            else
                vertex2.second = (float)(best_case_divisions.second) * (best_case_rects[i][0] + 1);
            
            edges.pb(mp(vertex1, vertex2));
            vertex1 = vertex2;

            if (reverse)
                vertex2.second = 0;
            else
                vertex2.first = 0;
        
            edges.pb(mp(vertex1, vertex2));
            vertex1 = vertex2;

            if (reverse)
                vertex2.first = (float)(best_case_rects[0][0] + 1) * (best_case_divisions.second) - 0.5;
            else
                vertex2.second = (float)(best_case_rects[0][0] + 1) * (best_case_divisions.second) - 0.5;
            
            edges.pb(mp(vertex1, vertex2));
        }
    }
    return edges;
}

int main()
{
    vector<int>acc;
    int sum=0;
    for (int iter = 0; iter < 10; iter++)
    {
        char filename[100] = "input00.txt";
        filename[6] = iter + '0';  

        freopen(filename, "r", stdin);

        vector<vector<int>> grid(10001, vector<int>(10001, 0)),prefix_vector_2d(10002, vector<int>(10002, 0));;
        ll sum_of_all_crystals = 0;

        int n;
        cin >> n;
        for (int i = 0; i < n; i++)
        {
            int x, y, c;
            cin >> x >> y >> c;
            sum_of_all_crystals += c;
            grid[x][y] += c;
        }

        int m;
        cin >> m;
        for (int i = 0; i < m; i++)
        {
            int x, y, c;
            cin >> x >> y >> c;
            grid[x][y] -= c;
        }

        computeprefix(prefix_vector_2d,grid);

        vector<int> factors;
        computefactors(factors,10000,2000);

        pair<int,int> best_case_divisionsx,best_case_divisionsy;
        vector<vector<int>> best_case_rectsx,best_case_rectsy;

        ll max_value_enclosedx=0,max_value_enclosedy=0,max_value_enclosed=0;
        for(auto factor1 : factors)
        {
            for (auto factor2 : factors)
            {
                for(int mtd=0; mtd<=1; mtd++)
                {
                    if(factor1>=200)
                        max_value_enclosedy=max(max_value_enclosedy,max_value_using_rectangles(0,mtd,factor1,factor2,max_value_enclosedy,160,best_case_divisionsy,best_case_rectsy,prefix_vector_2d));
                    else
                        max_value_enclosedy=max(max_value_enclosedy,max_value_using_rectangles(0,mtd,factor1,factor2,max_value_enclosedy,200,best_case_divisionsy,best_case_rectsy,prefix_vector_2d));

                    if(factor1>=200)
                        max_value_enclosedx=max(max_value_enclosedx,max_value_using_rectangles(1,mtd,factor1,factor2,max_value_enclosedx,160,best_case_divisionsx,best_case_rectsx,prefix_vector_2d));
                    else
                        max_value_enclosedx=max(max_value_enclosedx,max_value_using_rectangles(1,mtd,factor1,factor2,max_value_enclosedx,200,best_case_divisionsx,best_case_rectsx,prefix_vector_2d));
                }
            }
        }
        vector<pair<pair<float,float>,pair<float,float>>> edges,edges_x,edges_y;
        edges_x=create_edges(0,best_case_rectsx,best_case_divisionsx);

        swap(best_case_divisionsy.first,best_case_divisionsy.second);
        edges_y=create_edges(1,best_case_rectsy,best_case_divisionsy);
        
        if(max_value_enclosedx>=max_value_enclosedy)
            edges=edges_x;
        else
            edges=edges_y;
        
        max_value_enclosed=max(max_value_enclosedx,max_value_enclosedy);
        
        cout<<"Number of edges : "<<edges.size()<<endl;
        cout<<endl;
        for(auto [v1,v2] : edges){
           
            cout << "(" << v1.first << "," << v1.second << ") (" << v2.first << "," << v2.second << ")" <<endl;
        }
        cout<<endl;

        cout<<"Accuracy : "<<100*((double)max_value_enclosed)/sum_of_all_crystals<<endl;
        acc.pb(100*((double)max_value_enclosed)/sum_of_all_crystals);
        sum+=100*((double)max_value_enclosed)/sum_of_all_crystals;
    }
    cout<<"Average Accuracy : "<<((double)sum/acc.size());
    return 0;
}