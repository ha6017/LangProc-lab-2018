#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,    // The name of the register to put the result in
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match( program->type, reNum ) ){
        std::cout<<"const "<<destReg<<" "<<program->type<<std::endl;
        
    }else if( regex_match( program->type, reId ) ){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->type<<" "<<zero<<std::endl;

    }else if(program->type=="Param"){
        std::cout<<"param "<<destReg<<" "<<program->value<<std::endl;

    }else if(program->type=="Seq"){
        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        }
    }
    // TODO : handle the others
    else if( program->type == "Lessthan"){
        std::string A = makeName("A");
        std::string B = makeName("B");
        CompileRec(A, program->branches[0]);
        CompileRec(B, program->branches[1]);

        std::cout<<"lt "<<destReg<<" "<<A<<" "<<B<<std::endl;
    }
    else if( program->type == "Add"){
        std::string A = makeName("A");
        std::string B = makeName("B");
        CompileRec(A, program->branches[0]);
        CompileRec(B, program->branches[1]);

        std::cout<<"add "<<destReg<<" "<<A<<" "<<B<<std::endl;
    }
    else if( program->type == "Sub"){
        std::string A = makeName("A");
        std::string B = makeName("B");
        CompileRec(A, program->branches[0]);
        CompileRec(B, program->branches[1]);

        std::cout<<"sub "<<destReg<<" "<<A<<" "<<B<<std::endl;
    }
    else if( program->type == "Input"){
        //std::string Input = makeName("input");
        //CompileRec(Input, program->branches[0]);

        //std::cout<<"input "<<destReg<<" "<<Input<<std::endl;
        std::cout<<"input "<<destReg<<" "<<std::endl;
        
    }
    else if( program->type == "Output"){
        CompileRec(destReg, program->branches[0]);

        std::cout<<"output "<<destReg<<" "<<std::endl;
    }
    else if(program->type == "Assign"){
        std::string zero = makeName("zero");
        CompileRec(program->value, program->branches[0]);

        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->value<<" "<<zero<<std::endl;
    }
    else if(program->type == "If"){
        std::string C = makeName("Cond");
        std::string X = makeName("X");
        std::string Y = makeName("Y");

        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;

        CompileRec(C, program->branches[0]);
        std::cout<<"beq "<<C<<" "<<zero<<" "<<X<<std::endl;
        CompileRec(destReg, program->branches[1]);
        std::cout<<"beq "<<zero<<" "<<zero<<" "<<Y<<std::endl;
        std::cout<<":"<<X<<std::endl;
        CompileRec(destReg, program->branches[2]);
        std::cout<<":"<<Y<<std::endl;
    }

    else if(program->type == "While"){

        std::string C = makeName("Cond");
        std::string while_beg = makeName("while_beg");
        std::string while_end = makeName("while_end");


        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;

        std::cout<<":"<<while_beg<<std::endl;
        CompileRec(C, program->branches[0]);
        std::cout<<"beq "<<zero<<" "<<C<<" "<<while_end<<std::endl;

        CompileRec(destReg, program->branches[1]);
        std::cout<<"beq "<<zero<<" "<<zero<<" "<<while_beg<<std::endl;
        std::cout<<":"<<while_end<<std::endl;
    
    }

        
    else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

void Compile(
    TreePtr program
){
    // Create a register to hold the final result
    std::string res=makeName("res");
    
    // Compile the whole thing
    CompileRec(res, program);
    
    // Put the result out
    std::cout<<"halt "<<res<<"\n";
}
