//
//  LBJSONRenderFM.cpp
//  pluginTests
//
//  Created by Warren Tucker on 24/05/2016.
//  Copyright © 2016 Warren Tucker. All rights reserved.
//

#include "LBJSONRenderFM.h"
using namespace LB;

string LBJSONRenderFM::getVarString(){
    stringstream vString;
    for(SizeType i=0;i<varString.size();i++){
        if(i > 0 ){
            
            vString << "_" << varString[i];
            
        }else{
            vString << varString[i];
        }
    }
    
    return vString.str();
}

void LBJSONRenderFM::stepBack(SizeType p){
    
    for(SizeType i=1;i<=p;i++){
        varString.pop_back();
    }
    
    
}

void LBJSONRenderFM::processObject(const Value& doc){
    if(useLBDebug){
        printf("------ \n");
    }
    
    printf("Current Level %d\n",currentLevel);
    
    for (Value::ConstMemberIterator itr = doc.MemberBegin();
         itr != doc.MemberEnd(); ++itr)
    {
        printf("Current Level %d\n",currentLevel);
        
        bool intDone = false;
        if(kTypeNames[itr->value.GetType()] == kTypeNames[3]){ // Object
            if(useLBDebug){
                printf("Type of member processObject Object %s is %s\n",itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            }
            const rapidjson::Value& membersObj = doc[itr->name.GetString()];
            
            //if(varString.size() == 0){
            varString.push_back(itr->name.GetString());
            
            //}
            
            
            if(useLBDebug){
                printf("%s\n",this->getVarString().c_str());
            }
            currentLevel++;
            this->processObject(membersObj);
            currentLevel--;
            varString.pop_back();
            
        }else if(kTypeNames[itr->value.GetType()] == kTypeNames[4]){ // Array
            if(useLBDebug){
                printf("Type of member processObject Array %s is %s\n",itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            }
            string pVar = "";
            
            
            if(varString.size() > 0){
                pVar = this->getVarString();
            }
            
            stringstream arrayCount;
            arrayCount << doc[itr->name.GetString()].Size();
            stringstream arrayCountText;
            if(pVar != ""){
                arrayCountText << pVar.c_str() << "_" << itr->name.GetString() << "_count";
            }else{
                arrayCountText << itr->name.GetString() << "_count";
                
                
            }
            
            if(useLBDebug){
                printf("%s\n",arrayCountText.str().c_str());
                
                recordTest.insert(make_pair(arrayCountText.str(),arrayCount.str()));
            }
            record.insert(make_pair(arrayCountText.str(),arrayCount.str()));
            
            
            for (SizeType i = 0; i < doc[itr->name.GetString()].Size(); i++){
                multiItemsDone = false;
                const rapidjson::Value& membersArray = doc[itr->name.GetString()][i];
                
                stringstream arrayCountI;
                arrayCountI << i;
                if(doc[itr->name.GetString()][i].IsArray()){
                    if(varString.size() > 0){
                        
                        if(i==0){
                            varString.push_back(itr->name.GetString());
                            varString.push_back(arrayCountI.str());
                        }else{
                            
                            varString.push_back(arrayCountI.str());
                        }
                    }else{
                        
                        varString.push_back(itr->name.GetString());
                        varString.push_back(arrayCountI.str());
                    }
                    
                    if(useLBDebug){
                        printf("%s\n",this->getVarString().c_str());
                    }
                    currentLevel++;
                    this->processArray(membersArray);
                    currentLevel--;
                    varString.pop_back();
                }else if(doc[itr->name.GetString()][i].IsObject()){
                    
                    if(varString.size() > 0){
                        
                        if(i==0){
                            varString.push_back(itr->name.GetString());
                            varString.push_back(arrayCountI.str());
                        }else{
                            
                            varString.push_back(arrayCountI.str());
                        }
                    }else{
                        
                        varString.push_back(itr->name.GetString());
                        varString.push_back(arrayCountI.str());
                    }
                    
                    if(useLBDebug){
                        printf("%s\n",this->getVarString().c_str());
                    }
                    currentLevel++;
                    this->processObject(membersArray);
                    currentLevel--;
                    varString.pop_back();
                }else{
                    if(varString.size() > 0){
                        
                        if(i==0){
                            varString.push_back(itr->name.GetString());
                            varString.push_back(arrayCountI.str());
                        }else{
                            
                            varString.push_back(arrayCountI.str());
                        }
                    }else{
                        
                        varString.push_back(itr->name.GetString());
                        varString.push_back(arrayCountI.str());
                    }
                    
                    if(useLBDebug){
                        printf("%s\n",this->getVarString().c_str());
                    }
                    currentLevel++;
                    this->processValue(membersArray);
                    currentLevel--;
                    
                }
                
                
            }
            if(doc[itr->name.GetString()].Size() > 0){
                varString.pop_back();
            }
            
        }else{ // is a value
            if(useLBDebug){
                printf("Type of member processObject Value is %s", kTypeNames[itr->value.GetType()]);
            }
            
            if(itr->value.IsString()){
                if(useLBDebug){
                    printf(" %s = %s",itr->name.GetString(),itr->value.GetString());
                }
                
                LBValue << itr->value.GetString();
                varString.push_back(itr->name.GetString());
                LBName << this->getVarString();
                
                record.insert(make_pair(LBName.str(),LBValue.str()));
                
                
                if(useLBDebug){
                    printf("%s\n",this->getVarString().c_str());
                }
                recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                LBName.str(std::string());
                LBValue.str(std::string());
                varString.pop_back();
                
            }
            if(itr->value.IsNumber()){
                if(itr->value.IsInt()){
                    if(!intDone){
                        if(useLBDebug){
                            printf(" %s = %d ",itr->name.GetString(),itr->value.GetInt());
                        }
                        
                        LBValue << itr->value.GetInt();
                        varString.push_back(itr->name.GetString());
                        LBName << this->getVarString();
                        
                        record.insert(make_pair(LBName.str(),LBValue.str()));
                        
                        
                        if(useLBDebug){
                            printf("%s\n",this->getVarString().c_str());
                        }
                        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                        LBName.str(std::string());
                        LBValue.str(std::string());
                        varString.pop_back();
                        intDone = true;
                    }
                    //printf(" %s = %d ",itr->name.GetString(),itr->value.GetInt());
                }
                if(itr->value.IsDouble()){
                    if(useLBDebug){
                        printf(" %s = %f ",itr->name.GetString(),itr->value.GetDouble());
                    }
                    
                    LBValue << itr->value.GetDouble();
                    varString.push_back(itr->name.GetString());
                    LBName << this->getVarString();
                    
                    record.insert(make_pair(LBName.str(),LBValue.str()));
                    
                    
                    if(useLBDebug){
                        printf("%s\n",this->getVarString().c_str());
                    }
                    recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                    LBName.str(std::string());
                    LBValue.str(std::string());
                    varString.pop_back();
                }
                if(itr->value.IsInt64()){
                    if(!intDone){
                        if(useLBDebug){
                            printf(" %s = %lld ",itr->name.GetString(),itr->value.GetInt64());
                        }
                        
                        LBValue << itr->value.GetInt64();
                        varString.push_back(itr->name.GetString());
                        LBName << this->getVarString();
                        
                        record.insert(make_pair(LBName.str(),LBValue.str()));
                        
                        
                        if(useLBDebug){
                            printf("%s\n",this->getVarString().c_str());
                        }
                        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                        LBName.str(std::string());
                        LBValue.str(std::string());
                        varString.pop_back();
                    }
                    
                }
            }
            if(itr->value.IsNull()){
                if(useLBDebug){
                    printf(" %s = NULL ",itr->name.GetString());
                }
                
                LBValue << "Null";
                varString.push_back(itr->name.GetString());
                LBName << this->getVarString();
                
                record.insert(make_pair(LBName.str(),LBValue.str()));
                
                
                if(useLBDebug){
                    printf(" %s\n",this->getVarString().c_str());
                }
                recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                LBName.str(std::string());
                LBValue.str(std::string());
                varString.pop_back();
            }
            if(itr->value.IsBool()){
                if(useLBDebug){
                    printf(" %s = %d ",itr->name.GetString(),itr->value.GetBool());
                }
                
                LBValue << itr->value.GetBool();
                varString.push_back(itr->name.GetString());
                LBName << this->getVarString();
                
                record.insert(make_pair(LBName.str(),LBValue.str()));
                
                
                if(useLBDebug){
                    printf(" %s\n",this->getVarString().c_str());
                }
                recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                LBName.str(std::string());
                LBValue.str(std::string());
                varString.pop_back();
            }
            if(useLBDebug){
                printf("\n");
            }
            //
            
        }
        
    }
    //varString.clear();
    
    
}

void LBJSONRenderFM::processArray(const Value& doc){
    if(useLBDebug){
        printf("------\n");
    }
    bool intDone = false;
    
    
    
    if(doc.IsArray()){
        cout << "Test IS ARRAY\n" << endl;
        
    }else{
        
        
        for (Value::ConstMemberIterator itr = doc.MemberBegin();
             itr != doc.MemberEnd(); ++itr)
        {
            intDone = false;
            if(kTypeNames[itr->value.GetType()] == kTypeNames[3]){
                if(useLBDebug){
                    printf("Type of processArray Object %s is %s\n",itr->name.GetString(), kTypeNames[itr->value.GetType()]);
                }
                const rapidjson::Value& memberObj = itr->value;
                if(memberObj.IsObject()){
                    if(varString.size() > 0){
                        varString.push_back(itr->name.GetString());
                    }
                    
                    if(useLBDebug){
                        printf(" %s\n",this->getVarString().c_str());
                    }
                    
                    currentLevel++;
                    this->processObject(memberObj);
                    currentLevel--;
                    varString.pop_back();
                }else if(memberObj.IsArray()){
                    if(varString.size() > 0){
                        varString.push_back(itr->name.GetString());
                    }
                    
                    if(useLBDebug){
                        printf(" %s\n",this->getVarString().c_str());
                    }
                    currentLevel++;
                    this->processArray(memberObj);
                    currentLevel--;
                    varString.pop_back();
                }else{
                    for (SizeType i = 0; i < itr->value.Size(); i++){
                        intDone = false;
                        const rapidjson::Value& membersObj = itr->value[i];
                        if(membersObj.IsObject()){
                            if(varString.size() > 0){
                                varString.push_back(itr->name.GetString());
                            }
                            
                            if(useLBDebug){
                                printf(" %s\n",this->getVarString().c_str());
                            }
                            this->processObject(membersObj);
                        }else if(membersObj.IsArray()){
                            if(varString.size() > 0){
                                varString.push_back(itr->name.GetString());
                            }
                            
                            if(useLBDebug){
                                printf(" %s\n",this->getVarString().c_str());
                            }
                            currentLevel++;
                            this->processArray(membersObj);
                            currentLevel--;
                            varString.pop_back();
                        }else{
                            if(useLBDebug){
                                printf("Type of member processArray Object Value %s is %s",itr->name.GetString(), kTypeNames[itr->value.GetType()]);
                            }
                            if(itr->value.IsString()){
                                printf(" %s = %s ",itr->name.GetString(),itr->value.GetString());
                                
                                LBValue << itr->value.GetString();
                                varString.push_back(itr->name.GetString());
                                LBName << this->getVarString();
                                
                                record.insert(make_pair(LBName.str(),LBValue.str()));
                                
                                
                                if(useLBDebug){
                                    printf(" %s\n",this->getVarString().c_str());
                                }
                                recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                                LBName.str(std::string());
                                LBValue.str(std::string());
                                varString.pop_back();
                            }
                            if(itr->value.IsNumber()){
                                if(itr->value.IsInt()){
                                    if(!intDone){
                                        if(useLBDebug){
                                            printf("int %s = %d ",itr->name.GetString(),itr->value.GetInt());
                                        }
                                        
                                        LBValue << itr->value.GetInt();
                                        varString.push_back(itr->name.GetString());
                                        LBName << this->getVarString();
                                        
                                        record.insert(make_pair(LBName.str(),LBValue.str()));
                                        
                                        
                                        if(useLBDebug){
                                            printf(" %s\n",this->getVarString().c_str());
                                        }
                                        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                                        LBName.str(std::string());
                                        LBValue.str(std::string());
                                        varString.pop_back();
                                        intDone = true;
                                    }
                                    //printf(" %s = %d ",itr->name.GetString(),itr->value.GetInt());
                                }
                                if(itr->value.IsDouble()){
                                    if(useLBDebug){
                                        printf(" %s = %f ",itr->name.GetString(),itr->value.GetDouble());
                                    }
                                    
                                    LBValue << itr->value.GetDouble();
                                    varString.push_back(itr->name.GetString());
                                    LBName << this->getVarString();
                                    
                                    record.insert(make_pair(LBName.str(),LBValue.str()));
                                    
                                    
                                    if(useLBDebug){
                                        printf(" %s\n",this->getVarString().c_str());
                                    }
                                    recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                                    LBName.str(std::string());
                                    LBValue.str(std::string());
                                    varString.pop_back();
                                }
                                if(itr->value.IsInt64()){
                                    if(!intDone){
                                        if(useLBDebug){
                                            printf("int64 %s = %lld ",itr->name.GetString(),itr->value.GetInt64());
                                        }
                                        
                                        LBValue << itr->value.GetInt64();
                                        varString.push_back(itr->name.GetString());
                                        LBName << this->getVarString();
                                        
                                        record.insert(make_pair(LBName.str(),LBValue.str()));
                                        
                                        
                                        if(useLBDebug){
                                            printf(" %s\n",this->getVarString().c_str());
                                        }
                                        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                                        LBName.str(std::string());
                                        LBValue.str(std::string());
                                        varString.pop_back();
                                        
                                    }
                                    
                                }
                            }
                            if(itr->value.IsNull()){
                                if(useLBDebug){
                                    printf(" %s = NULL ",itr->name.GetString());
                                }
                                
                                LBValue << "Null";
                                varString.push_back(itr->name.GetString());
                                LBName << this->getVarString();
                                
                                record.insert(make_pair(LBName.str(),LBValue.str()));
                                
                                
                                if(useLBDebug){
                                    printf(" %s\n",this->getVarString().c_str());
                                }
                                recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                                LBName.str(std::string());
                                LBValue.str(std::string());
                                varString.pop_back();
                            }
                            if(itr->value.IsBool()){
                                if(useLBDebug){
                                    printf(" %s = %d ",itr->name.GetString(),itr->value.GetBool());
                                }
                                
                                LBValue << itr->value.GetBool();
                                varString.push_back(itr->name.GetString());
                                LBName << this->getVarString();
                                
                                record.insert(make_pair(LBName.str(),LBValue.str()));
                                
                                
                                if(useLBDebug){
                                    printf(" %s\n",this->getVarString().c_str());
                                }
                                recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                                LBName.str(std::string());
                                LBValue.str(std::string());
                                varString.pop_back();
                            }
                            printf("\n");
                        }
                        
                    }
                }
                
            }else if(kTypeNames[itr->value.GetType()] == kTypeNames[4]){
                if(useLBDebug){
                    printf("Type of member processArray Array %s is %s size %d \n",itr->name.GetString(), kTypeNames[itr->value.GetType()],itr->value.Size());
                }
                int num = 0;
                for (Value::ConstValueIterator itr1 = itr->value.Begin(); itr1 != itr->value.End(); ++itr1){
                    bool intDone = false;
                    //printf("Type of member processArray Array Value %s is %s",itr->name[i].GetString(), kTypeNames[itr->value[i].GetType()]);
                    
                    if(itr1->IsString()){
                        if(useLBDebug){
                            printf(" %s_%d = %s ",itr->name.GetString(),num,itr1->GetString());
                        }
                        
                        LBValue << itr1->GetString();
                        varString.push_back(itr->name.GetString());
                        LBName << this->getVarString();
                        
                        record.insert(make_pair(LBName.str(),LBValue.str()));
                        
                        
                        if(useLBDebug){
                            printf(" %s\n",this->getVarString().c_str());
                        }
                        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                        LBName.str(std::string());
                        LBValue.str(std::string());
                        varString.pop_back();
                    }
                    if(itr1->IsNumber()){
                        if(itr1->IsInt()){
                            if(!intDone){
                                if(useLBDebug){
                                    printf(" %s_%d = %d ",itr->name.GetString(),num,itr1->GetInt());
                                }
                                
                                LBValue << itr1->GetInt();
                                varString.push_back(itr->name.GetString());
                                LBName << this->getVarString();
                                
                                record.insert(make_pair(LBName.str(),LBValue.str()));
                                
                                
                                if(useLBDebug){
                                    printf(" %s\n",this->getVarString().c_str());
                                }
                                recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                                LBName.str(std::string());
                                LBValue.str(std::string());
                                varString.pop_back();
                                intDone = true;
                            }
                            //printf(" %s_%d = %d ",itr->name.GetString(),num,itr->value.GetInt());
                        }
                        if(itr1->IsDouble()){
                            if(useLBDebug){
                                printf(" %s_%d = %f ",itr->name.GetString(),num,itr1->GetDouble());
                            }
                            
                            LBValue << itr1->GetDouble();
                            varString.push_back(itr->name.GetString());
                            LBName << this->getVarString();
                            
                            record.insert(make_pair(LBName.str(),LBValue.str()));
                            
                            
                            if(useLBDebug){
                                printf(" %s\n",this->getVarString().c_str());
                            }
                            recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                            LBName.str(std::string());
                            LBValue.str(std::string());
                            varString.pop_back();
                        }
                        if(itr1->IsInt64()){
                            if(!intDone){
                                if(useLBDebug){
                                    printf(" %s_%d = %lld ",itr->name.GetString(),num,itr1->GetInt64());
                                }
                                
                                LBValue << itr1->GetInt64();
                                varString.push_back(itr->name.GetString());
                                LBName << this->getVarString();
                                
                                record.insert(make_pair(LBName.str(),LBValue.str()));
                                
                                
                                if(useLBDebug){
                                    printf(" %s\n",this->getVarString().c_str());
                                }
                                recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                                LBName.str(std::string());
                                LBValue.str(std::string());
                                varString.pop_back();
                            }
                        }
                        
                    }
                    if(itr1->IsNull()){
                        if(useLBDebug){
                            printf(" %s_%d = NULL ",itr->name.GetString(),num);
                        }
                        
                        LBValue << "Null";
                        varString.push_back(itr->name.GetString());
                        LBName << this->getVarString();
                        
                        record.insert(make_pair(LBName.str(),LBValue.str()));
                        
                        
                        if(useLBDebug){
                            printf(" %s\n",this->getVarString().c_str());
                        }
                        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                        LBName.str(std::string());
                        LBValue.str(std::string());
                        varString.pop_back();
                    }
                    if(itr1->IsBool()){
                        if(useLBDebug){
                            printf(" %s_%d = %d ",itr->name.GetString(),num,itr1->GetBool());
                        }
                        
                        LBValue << itr1->GetBool();
                        varString.push_back(itr->name.GetString());
                        LBName << this->getVarString();
                        
                        record.insert(make_pair(LBName.str(),LBValue.str()));
                        
                        
                        if(useLBDebug){
                            printf(" %s\n",this->getVarString().c_str());
                        }
                        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                        LBName.str(std::string());
                        LBValue.str(std::string());
                        varString.pop_back();
                    }
                    if(useLBDebug){
                        printf("\n");
                    }
                    
                    
                    num++;
                    
                    
                    
                    
                    
                }
                
            }else{
                if(useLBDebug){
                    printf("Type of member processArray Value is %s", kTypeNames[itr->value.GetType()]);
                }
                if(itr->value.IsString()){
                    printf(" %s = %s ",itr->name.GetString(),itr->value.GetString());
                    
                    LBValue << itr->value.GetString();
                    varString.push_back(itr->name.GetString());
                    LBName << this->getVarString();
                    
                    record.insert(make_pair(LBName.str(),LBValue.str()));
                    
                    
                    if(useLBDebug){
                        printf(" %s\n",this->getVarString().c_str());
                    }
                    recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                    LBName.str(std::string());
                    LBValue.str(std::string());
                    varString.pop_back();
                }
                if(itr->value.IsNumber()){
                    if(itr->value.IsInt()){
                        if(!intDone){
                            if(useLBDebug){
                                printf("int %s = %d ",itr->name.GetString(),itr->value.GetInt());
                            }
                            
                            LBValue << itr->value.GetInt();
                            varString.push_back(itr->name.GetString());
                            LBName << this->getVarString();
                            
                            record.insert(make_pair(LBName.str(),LBValue.str()));
                            
                            
                            if(useLBDebug){
                                printf(" %s\n",this->getVarString().c_str());
                            }
                            recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                            LBName.str(std::string());
                            LBValue.str(std::string());
                            varString.pop_back();
                            intDone = true;
                        }
                        //printf(" %s = %d ",itr->name.GetString(),itr->value.GetInt());
                    }
                    if(itr->value.IsDouble()){
                        if(useLBDebug){
                            printf(" %s = %f ",itr->name.GetString(),itr->value.GetDouble());
                        }
                        
                        LBValue << itr->value.GetDouble();
                        varString.push_back(itr->name.GetString());
                        LBName << this->getVarString();
                        
                        record.insert(make_pair(LBName.str(),LBValue.str()));
                        
                        
                        if(useLBDebug){
                            printf(" %s\n",this->getVarString().c_str());
                        }
                        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                        LBName.str(std::string());
                        LBValue.str(std::string());
                        varString.pop_back();
                    }
                    if(itr->value.IsInt64()){
                        if(!intDone){
                            if(useLBDebug){
                                printf("int64 %s = %lld ",itr->name.GetString(),itr->value.GetInt64());
                            }
                            
                            LBValue << itr->value.GetInt64();
                            varString.push_back(itr->name.GetString());
                            LBName << this->getVarString();
                            
                            record.insert(make_pair(LBName.str(),LBValue.str()));
                            
                            
                            if(useLBDebug){
                                printf(" %s\n",this->getVarString().c_str());
                            }
                            recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                            LBName.str(std::string());
                            LBValue.str(std::string());
                            varString.pop_back();
                        }
                    }
                }
                if(itr->value.IsNull()){
                    if(useLBDebug){
                        printf(" %s = NULL ",itr->name.GetString());
                    }
                    
                    LBValue << "Null";
                    varString.push_back(itr->name.GetString());
                    LBName << this->getVarString();
                    
                    record.insert(make_pair(LBName.str(),LBValue.str()));
                    
                    
                    if(useLBDebug){
                        printf(" %s\n",this->getVarString().c_str());
                    }
                    recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                    LBName.str(std::string());
                    LBValue.str(std::string());
                    varString.pop_back();
                }
                if(itr->value.IsBool()){
                    if(useLBDebug){
                        printf(" %s = %d ",itr->name.GetString(),itr->value.GetBool());
                    }
                    
                    LBValue << itr->value.GetBool();
                    varString.push_back(itr->name.GetString());
                    LBName << this->getVarString();
                    
                    record.insert(make_pair(LBName.str(),LBValue.str()));
                    
                    
                    if(useLBDebug){
                        printf(" %s\n",this->getVarString().c_str());
                    }
                    recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                    LBName.str(std::string());
                    LBValue.str(std::string());
                    varString.pop_back();
                }
                if(useLBDebug){
                    printf("\n");
                }
            }
        }
    }
    
}

void LBJSONRenderFM::processValue(const Value& doc){
    bool intDone = false;
    if(useLBDebug){
        printf("Type of member processValue Value is %s", kTypeNames[doc.GetType()]);
    }
    if(doc.IsString()){
        
        
        LBValue << doc.GetString();
        
        LBName << this->getVarString();
        
        record.insert(make_pair(LBName.str(),LBValue.str()));
        
        
        if(useLBDebug){
            printf(" %s\n",this->getVarString().c_str());
        }
        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
        LBName.str(std::string());
        LBValue.str(std::string());
        varString.pop_back();
    }
    if(doc.IsNumber()){
        if(doc.IsInt()){
            if(!intDone){
                if(useLBDebug){
                    printf(" %s = %d ",this->getVarString().c_str(),doc.GetInt());
                }
                
                LBValue << doc.GetInt();
                
                LBName << this->getVarString();
                
                record.insert(make_pair(LBName.str(),LBValue.str()));
                
                
                if(useLBDebug){
                    printf(" %s\n",this->getVarString().c_str());
                }
                recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                LBName.str(std::string());
                LBValue.str(std::string());
                varString.pop_back();
                intDone = true;
            }
            //printf(" %s = %d ",itr->name.GetString(),itr->value.GetInt());
        }
        if(doc.IsDouble()){
            
            
            LBValue << doc.GetDouble();
            
            LBName << this->getVarString();
            
            record.insert(make_pair(LBName.str(),LBValue.str()));
            
            
            if(useLBDebug){
                printf(" %s\n",this->getVarString().c_str());
            }
            recordTest.insert(make_pair(LBName.str(),LBValue.str()));
            LBName.str(std::string());
            LBValue.str(std::string());
            varString.pop_back();
            
        }
        if(doc.IsInt64()){
            if(!intDone){
                
                
                LBValue << doc.GetInt64();
                
                LBName << this->getVarString();
                
                record.insert(make_pair(LBName.str(),LBValue.str()));
                
                
                if(useLBDebug){
                    printf(" %s\n",this->getVarString().c_str());
                }
                recordTest.insert(make_pair(LBName.str(),LBValue.str()));
                LBName.str(std::string());
                LBValue.str(std::string());
            }
            
        }
    }
    if(doc.IsNull()){
        
        
        LBValue << "Null";
        
        LBName << this->getVarString();
        
        record.insert(make_pair(LBName.str(),LBValue.str()));
        
        
        if(useLBDebug){
            printf(" %s\n",this->getVarString().c_str());
        }
        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
        LBName.str(std::string());
        LBValue.str(std::string());
        varString.pop_back();
        
    }
    if(doc.IsBool()){
        
        
        
        LBValue << doc.GetBool();
        
        LBName << this->getVarString();
        
        record.insert(make_pair(LBName.str(),LBValue.str()));
        
        
        if(useLBDebug){
            printf(" %s\n",this->getVarString().c_str());
        }
        recordTest.insert(make_pair(LBName.str(),LBValue.str()));
        LBName.str(std::string());
        LBValue.str(std::string());
        varString.pop_back();
    }
    if(useLBDebug){
        printf("\n");
    }
}

string LBJSONRenderFM::sendDataToUrl(stringstream &postValues){
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    struct curl_slist *headers=NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    
    curl = curl_easy_init();
    if(curl) {
        string url = this->getRequestUrl();
        if(useLBDebug){
            cout << "Url to send: " << url.c_str() << endl;
        }
        string postVals = postValues.str();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postVals.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
    }else{
        readBuffer = "No Data";
        error.insert(make_pair("code","1007"));
        error.insert(make_pair("message","Error initialising the request"));
        errors.insert(make_pair(errorCount,error));
    }
    if(res == 0){
        return readBuffer;
    }else{
        error.insert(make_pair("code","1008"));
        stringstream errOut;
        errOut << "Error with sending the request. Request CODE " << res;
        error.insert(make_pair("message",errOut.str()));
        errors.insert(make_pair(errorCount,error));
        errorCount++;
        return "No Data";
    }
    return "No Data";
}

string LBJSONRenderFM::getRequestUrl(){
    if(!useDebug){
        return "https://www.sync2api.com/consumer/";
    }else{
        return "https://sync2api.linearblue.net/sync2api/website/consumer";
    }
}

void LBJSONRenderFM::processDocument(Document & doc){
    
    
    
    if(doc.IsObject()){	// Document is a JSON value represents the root of DOM. Root can be either an object or array.
        if(useLBDebug){
            printf("---------------- Start Object -------------------\n\n");
        }
        this->processObject(doc);
        records.insert(make_pair(0,record));
        record.clear();
        recordsTest.insert(make_pair(0,recordTest));
        recordTest.clear();
        if(useLBDebug){
            printf("---------------- End Object -------------------\n\n");
        }
    }
    
    if(doc.IsArray()){
        //int rec = 0;
        //for (SizeType i = 0; i < 1; i++){
        for (SizeType i = 0; i < doc.Size(); i++){ // rapidjson uses SizeType instead of size_t.
            if(useLBDebug){
                printf("---------------- Start Array -------------------\n\n");
            }
            const rapidjson::Value& membersObj = doc[i];
            assert(membersObj.IsObject());
            this->processObject(membersObj);
            records.insert(make_pair(i,record));
            record.clear();
            recordsTest.insert(make_pair(i,recordTest));
            recordTest.clear();
            varString.clear();
            
            if(useLBDebug){
                printf("---------------- End Array -------------------\n\n");
            }
        }
    }
    
    if(useLBDebug){
        //cout << outputVars.str() << endl;
        // cout << "\n\n" << endl;
        cout << outputVarsTest.str().c_str() << endl;
    }
    
}

void LBJSONRenderFM::addComplexPropertyToRequestList(){
    if(useLBDebug){
        cout << complexProperty.size() << endl;
    }
    stringstream setProperty;
    for(std::vector<int>::size_type i = 0; i != complexProperty.size(); i++) {
        if((i + 1) == complexProperty.size()){
            this->setProperty(setProperty.str(), complexProperty[i]);
            setProperty << "=" << complexProperty[i];
        }else{
            if(i == 0){
                setProperty << complexProperty[i];
            }else{
                setProperty << "[" << complexProperty[i] << "]";
            }
            
        }
        
    }
    complexProperty.clear();
    if(useLBDebug){
        cout << setProperty.str() << endl;
    }
}

LBJSONRenderFM::LBJSONRenderFM(){
    init = true;
    this->setProperty("outputType","json");
}

LBJSONRenderFM::LBJSONRenderFM(const LBJSONRenderFM & jO){
    init = true;
    useDebug = jO.useDebug;
    useLBDebug = jO.useLBDebug;
    module = jO.module;
    method = jO.method;
    cKey = jO.cKey;
    scKey = jO.scKey;
    records=jO.records;
    
    errors = jO.errors;
    
    postVars = jO.postVars;
    
}

stringstream LBJSONRenderFM::getRecordVarNames()
{
    stringstream recordVarNames;
    map<string,string>::iterator it;
    map<string,string> rec = this->getAllProperties(0);
    
    for ( it=rec.begin() ; it != rec.end(); it++ ) {
        recordVarNames <<  (*it).first << "\n";
    }
    return recordVarNames;
}

stringstream LBJSONRenderFM::getRecordVarNames(int recNum)
{
    stringstream recordVarNames;
    map<string,string>::iterator it;
    map<string,string> rec = this->getAllProperties(recNum);
    
    for ( it=rec.begin() ; it != rec.end(); it++ ) {
        recordVarNames <<  (*it).first << "\n";
    }
    return recordVarNames;
}

void LBJSONRenderFM::printRecords()
{
    map<int,map<string,string>>::iterator it;
    map<string,string>::iterator inner_it;
    
    for ( it=records.begin() ; it != records.end(); it++ ) {
        if(useLBDebug){
            cout << "\n\nNew record\n" << (*it).first << endl;
        }
        for( inner_it=(*it).second.begin(); inner_it != (*it).second.end(); inner_it++){
            if(useLBDebug){
                cout << (*inner_it).first << " => " << (*inner_it).second << endl;
            }
        }
    }
}

void LBJSONRenderFM::printTestRecords()
{
    map<int,map<string,string>>::iterator it;
    map<string,string>::iterator inner_it;
    
    for ( it=recordsTest.begin() ; it != recordsTest.end(); it++ ) {
        if(useLBDebug){
            cout << "\n\nNew record\n" << (*it).first << endl;
        }
        for( inner_it=(*it).second.begin(); inner_it != (*it).second.end(); inner_it++){
            if(useLBDebug){
                cout << (*inner_it).first << " => " << (*inner_it).second << endl;
            }
        }
    }
}

void LBJSONRenderFM::printErrors()
{
    map<int,map<string,string>>::iterator it;
    map<string,string>::iterator inner_it;
    
    for ( it=errors.begin() ; it != errors.end(); it++ ) {
        if(useLBDebug){
            cout << "\n\nNew record\n" << (*it).first << endl;
        }
        for( inner_it=(*it).second.begin(); inner_it != (*it).second.end(); inner_it++){
            if(useLBDebug){
                cout << (*inner_it).first << " => " << (*inner_it).second << endl;
            }
        }
    }
}

bool LBJSONRenderFM::hasErrors(){
    if(errors.size() > 0){
        return true;
    }else{
        return false;
    }
}

string LBJSONRenderFM::getErrors()
{
    map<int,map<string,string>>::iterator it;
    map<string,string>::iterator inner_it;
    stringstream errorOutput;
    if(errors.size() > 0){
        for ( it=errors.begin() ; it != errors.end(); it++ ) {
            
            for( inner_it=(*it).second.begin(); inner_it != (*it).second.end(); inner_it++){
                
                errorOutput << (*inner_it).first << " => " << (*inner_it).second << "\n";
                
            }
            errorOutput << "\n\n";
        }
        
        return errorOutput.str();
    }else{
        return "No Errors";
    }
}

string LBJSONRenderFM::getRecordCount(){
    stringstream size;
    size << records.size();
    return size.str();
}

string LBJSONRenderFM::getProperty(int rec,string prop){
    return records[rec].find(prop)->second;
}

map<string,string> LBJSONRenderFM::getAllProperties(int rec){
    return records[rec];
}

void LBJSONRenderFM::setLBDebug(bool t){
    useLBDebug = t;
}

void LBJSONRenderFM::setModule(string mod){
    module = mod;
}

void LBJSONRenderFM::setMethod(string meth){
    method = meth;
}

void LBJSONRenderFM::setProperty(string name,string value){
    postVars.insert(make_pair(name,value));
}

void LBJSONRenderFM::setLicenceKeys(string cK,string scK){
    cKey = cK;
    scKey = scK;
}

void LBJSONRenderFM::setDebug(bool t){
    
    useDebug = t;
    
}

void LBJSONRenderFM::printRequestVars(){
    stringstream postVals;
    map<string,string>::iterator it;
    for ( it=postVars.begin(); it != postVars.end(); it++ ) {
        if(it == postVars.begin()){
            postVals << (*it).first << "=" << (*it).second;
        }else{
            if(it != postVars.end()){
                postVals << "&" << (*it).first << "=" << (*it).second;
            }else{
                postVals << (*it).first << "=" << (*it).second;
            }
        }
    }
    if(useLBDebug){
        cout << postVals.str() << endl;
    }
}

void LBJSONRenderFM::clearRequest(){
    postVars.clear();
    errors.clear();
    error.clear();
    errorCount=0;
}

void LBJSONRenderFM::sendRequest(){
    records.clear();
    Document document;
    stringstream postVals;
    if(cKey == ""){
        error.insert(make_pair("code","1001"));
        error.insert(make_pair("message","sKey licence key is missing"));
        errors.insert(make_pair(errorCount,error));
        errorCount++;
    }
    if(scKey == ""){
        error.insert(make_pair("code","1002"));
        error.insert(make_pair("message","scKey licence key is missing"));
        errors.insert(make_pair(errorCount,error));
        error.clear();
        errorCount++;
    }
    if(module == ""){
        error.insert(make_pair("code","1003"));
        error.insert(make_pair("message","Module type is missing"));
        errors.insert(make_pair(errorCount,error));
        error.clear();
        errorCount++;
    }
    if(method == ""){
        error.insert(make_pair("code","1004"));
        error.insert(make_pair("message","Method is missing"));
        errors.insert(make_pair(errorCount,error));
        error.clear();
        errorCount++;
    }
    this->setProperty("cKey",cKey);
    this->setProperty("scKey",scKey);
    this->setProperty("module",module);
    this->setProperty("method",method);
    this->setProperty("outputType","json");
    
    map<string,string>::iterator it;
    
    for ( it=postVars.begin(); it != postVars.end(); it++ ) {
        if(it == postVars.begin()){
            postVals << (*it).first << "=" << (*it).second;
        }else{
            if(it != postVars.end()){
                postVals << "&" << (*it).first << "=" << (*it).second;
            }else{
                postVals << (*it).first << "=" << (*it).second;
            }
        }
    }
    
    if(useLBDebug){
        this->printRequestVars();
    }
    
    string data = this->sendDataToUrl(postVals);
    
    cout << data << endl;
    const char *json = data.c_str();
#if 0
    // "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
    if (document.Parse<0>(json).HasParseError()){
        
        error.insert(make_pair("code","1005"));
        error.insert(make_pair("message","Faild to Parse JSON object"));
        errors.insert(make_pair(errorCount,error));
        errorCount++;
        
    }
#else
    // In-situ parsing, decode strings directly in the source string. Source must be string.
    
    char *buffer;
    buffer = const_cast<char *>(json);
    
    if (document.ParseInsitu<0>(buffer).HasParseError()){
        
        error.insert(make_pair("code","1006"));
        error.insert(make_pair("message","Faild to Parse JSON object"));
        errors.insert(make_pair(errorCount,error));
        error.clear();
        errorCount++;
    }
#endif
    
    if(errorCount ==0){
        this->processDocument(document);
        if(useLBDebug){
            //this->printRecords();
        }
    }else{
        if(useLBDebug){
            cout << errorCount << endl;
            this->printErrors();
        }
    }
    
}

void LBJSONRenderFM::setComplexProperty(string property){
    
    complexProperty.push_back(property);
    
}

void LBJSONRenderFM::setComplexProperty(string property , int isProperty ){
    if(isProperty ==1){
        complexProperty.push_back(property);
        
        this->addComplexPropertyToRequestList();
    }else{
        error.insert(make_pair("code","1007"));
        error.insert(make_pair("message","Faild to set value for complex property"));
        errors.insert(make_pair(errorCount,error));
        error.clear();
        errorCount++;
    }
    
}
