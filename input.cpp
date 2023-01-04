#include"input.h"


void split(const std::string& str, 
           std::vector<std::string>& tokens, 
           const char delim ) {
    tokens.clear();
    
    std::istringstream iss(str);
    std::string tmp;
    while (std::getline(iss, tmp, delim)) {
        if (tmp != "") {
            // 如果两个分隔符相邻，则 tmp == ""，忽略。
            tokens.emplace_back(std::move(tmp));
        }
    }
}


TotalNode* readFile(string filename){
    cout<<"r 1"<<endl;
    char buf[1010]={0};
    cout<<"r 2"<<endl;
    try{
        cout<<"r22"<<endl;
        ifstream f(filename,ios::in);
        cout<<"r 3"<<endl;
//        f.open(filename,ios::in);
        cout<<"r 4"<<endl;
        if(!f){
            cerr<<"can not open file"<<endl;
        }

        int flag = 0, en = 1; //en is the number of elements
        vector<POINT> vp; //存储节点信息
        vector<ELEMENT> ve; //存储单员信息
        while(f.getline(buf,sizeof(buf))){
            string s = string(buf);

            if(s.find("******* E L E M E N T S *************")!=string::npos){
                continue;
            }
            if(s.find("NODE")!=string::npos||s.find("Node")!=string::npos) { //从*node开始读取节点信息
                flag = 1;
                continue;
            }

            if((s.find("ELEMENT")!=string::npos&&s.find("Surface")!=string::npos)||s.find("Element")!=string::npos) { //从*element开始读取单元信息
                flag = 2;
                cout<<s<<endl;
                continue;
            }
            if(s.find("END")!=string::npos||s.find("End")!=string::npos) //读取完毕
                break;
            if(flag == 0) continue; //没有标记的行，跳过
            if(flag==1){ //读取节点信息
                POINT p;
                vector<string> sv;
                split(s,sv,',');
                p.index = atoi(sv[0].c_str());
                p.x = atof(sv[1].c_str());
                p.y = atof(sv[2].c_str());
                vp.push_back(p);

                // cout<<s<<endl;
            }
            if(flag==2){ //读取单元信息
                ELEMENT e;
                vector<string> sv;
                split(s,sv,',');
    //            e.index = atoi(sv[0].c_str());
                e.index = en++;
                e.node1 = atoi(sv[1].c_str());
                e.node2 = atoi(sv[2].c_str());
                e.node3 = atoi(sv[3].c_str());
                ve.push_back(e);
                // cout<<s<<endl;
            }

        }
        //把读取的信息存储到数组中
        vector<Element*> v;
        vector<Node*> vn;
        for(int i=0;i<vp.size();i++){
            Node *n = new Node();
            n->index = vp[i].index;
            n->x = vp[i].x;
            n->y = vp[i].y;
            vn.push_back(n);
        }
        for(int i=0;i<ve.size();i++){
            int nodes[3] = {ve[i].node1-1,ve[i].node2-1,ve[i].node3-1};
            Element *e = new Element();
            e->index = ve[i].index;
            e->nodes[0] = vn[nodes[0]];
            e->nodes[1] = vn[nodes[1]];
            e->nodes[2] = vn[nodes[2]];
            v.push_back(e);
        }


        TotalNode *t = new TotalNode(vp.size(),v,vn);
        cout<<"read file success"<<endl;
        f.close();
        return t;
    }
    catch(std::exception &e) {
        cout<<e.what()<<endl;
        return NULL;
    }

}
