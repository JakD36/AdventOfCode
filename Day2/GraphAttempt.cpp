#include <stdio.h>
#include <vector>
#include <set>

using namespace std;
struct op;
typedef op* p_op;

int RunIntCode(int* intCode);

p_op Add(vector<p_op>* code, p_op a, p_op b);
p_op AddRecursive(vector<p_op>* code, p_op a, p_op b);

p_op Multiply(vector<p_op>* code, p_op a, p_op b);
p_op MultiplyRecursive(vector<p_op>* code, p_op a, p_op b);

bool HasChildren(p_op code);

void PrintEquation(vector<p_op>* code, p_op node);

struct op
{
    int dest;
    int operation;
    bool hasValue;
    p_op children[2];
};

int main()
{
    char filepath[] = "/Users/Jack/input.txt";
    FILE* file = fopen(filepath,"r");
    
    vector<p_op> code;
    
    int val;
    while(fscanf(file,"%d",&val) != EOF)
    {
        p_op x = new op();
        x->hasValue = true;
        x->dest = val;
        code.push_back(x);
        fscanf(file,",");
    }
    
    code[1] = new op();
    code[1]->hasValue = false;
    code[1]->dest = 0;
    
    code[2] = new op();
    code[2]->hasValue = false;
    code[2]->dest = 0;
    
    // Build new opcode graph
    // Collect old ops
    int i = 0;
    while(code[i]->dest != 99)
    {
        switch (code[i]->dest)
        {
            case 1:
                code[code[i+3]->dest] = Add(&code,code[i+1],code[i+2]);
                
                break;
            case 2:
                code[code[i+3]->dest] = Multiply(&code, code[i+1],code[i+2]);
                break;
        }
        if(!code[code[i+3]->dest]->hasValue)
        {
            printf("%d = ",code[i+3]->dest);
            PrintEquation(&code,code[code[i+3]->dest]);
            printf("\n");
        }
        i+=4;
    }
    
    if(code[0]->hasValue)
    {
        printf("0 has value\n");
    }
    else
    {
        printf("0 has no value\n");
    }
    if(HasChildren(code[0]))
    {
        printf("0 has children\n");
    }
    else
    {
        printf("0 has no children\n");
    }
    
    i = 19;
    printf("%d = ",i);
    PrintEquation(&code,code[i]);
    printf("\n");
    
    // printf("%d\n",code[0]->dest);
    // delete [] code;
    // int output = RunIntCode(intCode.data());
    // printf("%d\n",output);
    return 0;
}

p_op Add(vector<p_op>* code, p_op a, p_op b)
{
    p_op out = AddRecursive(code,a,b);
    if(out == NULL)
    {
        out = new op();
        out->operation = 1;
        out->hasValue = false;
        out->children[0] = a;
        out->children[1] = b;
    }
    return out;
}

p_op AddRecursive(vector<p_op>* code, p_op a, p_op b)
{
    if(a->hasValue && b->hasValue)
    {
        if((*code)[a->dest]->hasValue && (*code)[b->dest]->hasValue)
        {
            p_op out = new op();
            out->hasValue = true;
            out->dest = (*code)[a->dest]->dest + (*code)[b->dest]->dest;
            return out;
        }
        else
        {
            p_op out;
            out = AddRecursive(code,(*code)[a->dest],(*code)[b->dest]);
            return out;
        }
    }
    else if(a->hasValue && !b->hasValue)
    {
        if(!HasChildren(b))
        {
            return NULL;
        }
        else
        {
            p_op out = AddRecursive(code,a,b->children[0]);
            if(out == NULL)
            {
                out = AddRecursive(code,a,b->children[1]);
            }
            return out;
        }
    }
    else if(!a->hasValue && !b->hasValue)
    {
        if(!HasChildren(a))
        {
            return NULL;
        }
        else
        {
            p_op out = AddRecursive(code,b,a->children[0]);
            if(out == NULL)
            {
                out = AddRecursive(code,b,a->children[1]);
            }
            return out;
        }
    }
    else
    {
        bool aOp = HasChildren(a);
        bool bOp = HasChildren(b);
        
        if(aOp && bOp) // a is op b is op
        {
            p_op out;
            out = AddRecursive(code,a,b->children[0]);
            out = AddRecursive(code,out,b->children[1]);
            return out;
        }
        else if(!aOp && bOp) // a is symbol b is op
        {
            p_op out;
            out = AddRecursive(code,a,b->children[0]);
            if(out == NULL)
            {
                out = AddRecursive(code,a,b->children[1]);
            }
            return out;
        }
        else if(aOp && !bOp) // a is op b is symbol
        {
            p_op out;
            out = AddRecursive(code,b,a->children[0]);
            if(out == NULL)
            {
                out = AddRecursive(code,b,a->children[1]);
            }
            return out;
        }
        else // a is symbol b is symbol
        {
            if(a == b)
            { // TODO: Check probably handling this wrong with the multiplication
                p_op out = new op();
                out->hasValue = false;
                out->operation = 2;
                out->children[0] = a;
                
                p_op child = new op();
                child->hasValue = true;
                child->dest = (*code).size();
                
                p_op newMemory = new op();
                newMemory->hasValue = true;
                newMemory->dest = 2;
                code->push_back(newMemory);
                
                out->children[0] = child;
                return out;
            }
            return NULL;
        }
    }
    
    return NULL;
}

#pragma region Multiplication
p_op Multiply(vector<p_op>* code, p_op a, p_op b)
{
    p_op out = MultiplyRecursive(code,a,b);
    if(out == NULL)
    {
        out = new op();
        out->operation = 2;
        out->hasValue = false;
        out->children[0] = a;
        out->children[1] = b;
    }
    return out;
}

p_op MultiplyRecursive(vector<p_op>* code, p_op a, p_op b)
{
    if(a->hasValue && b->hasValue)
    {
        p_op out = new op();
        if((*code)[a->dest]->hasValue && (*code)[b->dest]->hasValue)
        {
            out->hasValue = true;
            out->dest = (*code)[a->dest]->dest * (*code)[b->dest]->dest;
        }
        else
        {
            out = MultiplyRecursive(code,(*code)[a->dest],(*code)[b->dest]);
        }
        return out;
    }
    else if(a->hasValue && !b->hasValue)
    {
        if(!HasChildren(b))
        {
            return NULL;
        }
        else
        {
            p_op out = MultiplyRecursive(code,a,b->children[0]);
            if(out == NULL)
            {
                out = MultiplyRecursive(code,a,b->children[1]);
            }
            return out;
        }
    }
    else if(!a->hasValue && b->hasValue)
    {
        if(!HasChildren(b))
        {
            return NULL;
        }
        else
        {
            p_op out = MultiplyRecursive(code,b,a->children[0]);
            if(out == NULL)
            {
                out = MultiplyRecursive(code,b,a->children[1]);
            }
            return out;
        }
    }
    else // Neither A or b have vals could be symbols could be operations of symbols
    {
        bool aOp = HasChildren(a);
        bool bOp = HasChildren(b);
        
        if(aOp && bOp) // a is op b is op
        {
            if(a->operation == 1 && b->operation == 1) // a is add b is add
            {
                // a[0]b[0] + a[0]b[1] + a[1]b[0] + a[1]b[1]
                p_op outA;
                p_op outB;
                p_op outC;
                p_op outD;
                
                outA = Multiply(code, a->children[0],b->children[0]);
                outB = Multiply(code, a->children[0],b->children[1]);
                
                outC = Multiply(code, a->children[1],b->children[0]);
                outD = Multiply(code, a->children[1],b->children[1]);
                
                return Add(code,Add(code,outA,outB),Add(code,outC,outD));
            }
            else if(a->operation == 1 && b->operation == 2)
            {
                // a[0]b + a[1]b
                p_op outA;
                p_op outB;
                
                outA = Multiply(code, a->children[0],b);
                outB = Multiply(code, a->children[1],b);
                
                return Add(code,outA,outB);
            }
            else if(a->operation == 2 && b->operation == 1)
            {
                // ab[0] + ab[1]
                p_op outA;
                p_op outB;
                
                outA = Multiply(code, a,b->children[1]);
                outB = Multiply(code, a,b->children[1]);
                
                return Add(code,outA,outB);
            }
            else if(a->operation == 2 && b->operation == 2)
            {
                p_op out = MultiplyRecursive(code, a, b->children[0]);
                if(out == NULL)
                {
                    p_op out = MultiplyRecursive(code, a, b->children[1]);
                    if(out==NULL)
                        return NULL;
                    else
                    {
                        p_op out2 = new op();
                        out2->hasValue = false;
                        out2->children[0] = out;
                        out2->children[1] = b->children[0];
                        return out2;
                    }
                }
                return out;
            }
        }
        else if(!aOp && bOp) // a is symbol b is op
        {
            p_op out;
            if(b->operation == 2)
            {
                out = MultiplyRecursive(code,a,b->children[0]);
                if(out == NULL)
                {
                    out = MultiplyRecursive(code,a,b->children[1]);
                }
                return out;
            }
            else
            {
                out = MultiplyRecursive(code, a, b->children[0]);
                if(out == NULL)
                {
                    out = MultiplyRecursive(code, a, b->children[1]);
                    if(out==NULL)
                        return NULL;
                    else{
                        return Multiply(code,a,b->children[0]);
                    }
                }
                else
                {
                    out = Multiply(code, a, out->children[1]);
                    return out;
                }
            }
            return out;
        }
        else if(aOp && !bOp) // a is op b is symbol
        {
            p_op out;
            if(a->operation == 2)
            {
                out = MultiplyRecursive(code,b,a->children[0]);
                if(out == NULL)
                {
                    out = MultiplyRecursive(code,b,a->children[1]);
                }
                return out;
            }
            else
            {
                out = MultiplyRecursive(code, b, a->children[0]);
                if(out == NULL)
                {
                    out = MultiplyRecursive(code, b, a->children[1]);
                    if(out==NULL)
                        return NULL;
                    else{
                        return Multiply(code,b,a->children[0]);
                    }
                }
                else
                {
                    out = Multiply(code, b, out->children[1]);
                }
            }
            return out;
        }
        else // a is symbol b is symbol
        {
            return NULL;
        }
    }
    return NULL;
}

#pragma endregion

bool HasChildren(p_op node)
{
    return node->children[0] != NULL && node->children[1] != NULL;
}

void PrintEquation(vector<p_op>* code, p_op node)
{
    if(node->hasValue)
    {
        printf(" %d ",node->dest);
    }
    else if(node == (*code)[1])
    {
        printf(" XXX ");
    }
    else if(node == (*code)[2])
    {
        printf(" YYY ");
    }
    else if(HasChildren(node))
    {
        printf("( ");
        PrintEquation(code,node->children[0]);
        
        if(node->operation == 1)
            printf(" + ");
        else
            printf(" * ");
        
        PrintEquation(code,node->children[1]);
        printf(" )");
    }
}

int RunIntCode(int* intCode)
{
    int i = 0;
    while(intCode[i] != 99)
    {
        switch (intCode[i])
        {
            case 1:
                intCode[intCode[i+3]] = intCode[intCode[i + 1]] + intCode[intCode[i + 2]];
                break;
            case 2:
                intCode[intCode[i+3]] = intCode[intCode[i + 1]] * intCode[intCode[i + 2]];
                break;
        }
        i+=4;
    }
    return intCode[0];
}
