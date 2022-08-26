#include <algorithm>
#include <queue>

#include "basic.hpp"

bool isFraction(const std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: isFraction: the input expr is nullptr!\n");
        return false;
    }
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if (op == '/')
        {
            return true;
        }
    }
    return false;
}

std::unique_ptr<ExprAST> getNumerator(const std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: getNumerator: the input expr is nullptr!\n");
        return nullptr;
    }
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if (op == '/')
        {
            return std::move(binOpPtr->getLHS());
        }
    }
    fprintf(stderr, "\tWARNING: getNumerator: the input expr is not match for fraction\n");
    return expr->Clone();
}

std::unique_ptr<ExprAST> getDenominator(const std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: getDenominator: the input expr is nullptr!\n");
        return nullptr;
    }
    if (expr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();
        if (op == '/')
        {
            return std::move(binOpPtr->getRHS());
        }
    }
    fprintf(stderr, "\tWARNING: getDenominator: the input expr is not match for fraction\n");
    return expr->Clone();
}

std::unique_ptr<ExprAST> createBinaryExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2, const char op)
{
    if (expr1 == nullptr || expr2 == nullptr)
    {
        fprintf(stderr, "\tERROR: createBinaryExpr: the input contain nullptr!");
        return nullptr;
    }

    auto lhs = expr1->Clone();
    auto rhs = expr2->Clone();
    return std::make_unique<BinaryExprAST>(op, std::move(lhs), std::move(rhs));
}

std::unique_ptr<ExprAST> addExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2) { return createBinaryExpr(expr1, expr2, '+'); }

std::unique_ptr<ExprAST> mulExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2) { return createBinaryExpr(expr1, expr2, '*'); }

std::unique_ptr<ExprAST> divExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2) { return createBinaryExpr(expr1, expr2, '/'); }

std::unique_ptr<ExprAST> combineFraction(const std::unique_ptr<ExprAST> &numerator, const std::unique_ptr<ExprAST> &denominator)
{
    // init
    std::unique_ptr<ExprAST> numeratorNew = nullptr;
    std::unique_ptr<ExprAST> denominatorNew = nullptr;
    std::unique_ptr<ExprAST> numeratorFinal = nullptr;
    std::unique_ptr<ExprAST> denominatorFinal = nullptr;
    // numerator
    if (isFraction(numerator))
    {
        const std::unique_ptr<ExprAST> numeratorTmp = getNumerator(numerator);
        const std::unique_ptr<ExprAST> denominatorTmp = getDenominator(numerator);

        numeratorNew = numeratorTmp->Clone();
        denominatorNew = mulExpr(denominator, denominatorTmp);
    }
    else
    {
        // fprintf(stderr, "\tNOTE: combineFraction: the input numerator is not a fraction.\n");
        numeratorNew = numerator->Clone();
        denominatorNew = denominator->Clone();
    }
    // denominator
    if (isFraction(denominator))
    {
        const std::unique_ptr<ExprAST> numeratorTmp = getNumerator(denominatorNew);
        const std::unique_ptr<ExprAST> denominatorTmp = getDenominator(denominatorNew);

        numeratorFinal = mulExpr(numeratorNew, denominatorTmp);
        denominatorFinal = numeratorTmp->Clone();
    }
    else
    {
        // fprintf(stderr, "\tNOTE: combineFraction: the input denominator is not a fraction.\n");
        numeratorFinal = numeratorNew->Clone();
        denominatorFinal = denominatorNew->Clone();
    }

    return divExpr(numeratorFinal, denominatorFinal);
}

std::vector<std::unique_ptr<ExprAST>> extractItems(const std::unique_ptr<ExprAST> &expr)
{
    // fprintf(stderr, "extractItems: start--------\n");
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: extractItems: the input expr is nullptr!\n");
        return {};
    }
    std::string exprType = expr->type();
    std::vector<std::unique_ptr<ExprAST>> items;
    if (exprType != "Binary")
    { // eg: poly := exp(x)
        std::unique_ptr<ExprAST> exprTmp = expr->Clone();
        items.push_back(std::move(exprTmp));

        // print info for debug
        // fprintf(stderr, "\textractItems: expr is monomial\n");
        // fprintf(stderr, "\textractItems: No.0: %s\n", PrintExpression(items.at(0)).c_str());
        // fprintf(stderr, "extractItems: end----------\n");
        return items;
    }
    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    while (op == '+')
    {
        // rhs
        std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
        std::unique_ptr<ExprAST> exprTmp = rhs->Clone();
        items.push_back(std::move(exprTmp));
        // lhs
        std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
        std::string lhsType = lhs->type();
        if (lhsType == "Binary")
        { // the usual case
            binOpPtr = dynamic_cast<BinaryExprAST *>(lhs.get());
            op = binOpPtr->getOp();
        }
        else
        {
            exprTmp = lhs->Clone();
            items.push_back(std::move(exprTmp));
            break;
        }
    }
    if (op != '+')
    {
        std::unique_ptr<ExprAST> exprTmp = binOpPtr->Clone();
        items.push_back(std::move(exprTmp));
    }
    reverse(items.begin(), items.end());
    // print info for debug
    // fprintf(stderr, "\textractItems: expr size = %ld\n", items.size());
    // for (long unsigned int i = 0; i < items.size(); i++)
    // {
    //     fprintf(stderr, "\textractItems: No.%lu: %s\n", i, PrintExpression(items[i]).c_str());
    // }

    // fprintf(stderr, "extractItems: end----------\n");
    return items;
}

std::unique_ptr<ExprAST> moveDivKernel(const std::unique_ptr<ExprAST> &expr)
{                                          // level Traversal
    std::queue<BinaryExprAST *> nodeQueue; // nodeQueue存储层序遍历的节点
    std::queue<char> sideQueue;            // sideQueue只能存储'L'和'R',表示对应节点在根节点的左边还是右边
    std::unique_ptr<ExprAST> in = expr->Clone();
    const std::string exprType = expr->type();
    if (exprType == "Binary")
    {
        BinaryExprAST *root = dynamic_cast<BinaryExprAST *>(in.get());
        std::unique_ptr<ExprAST> &rootLhs = root->getLHS();
        std::unique_ptr<ExprAST> &rootRhs = root->getRHS();

        if (in != nullptr)
            nodeQueue.push(root);

        if ((rootLhs->type()) == "Binary")
            sideQueue.push('L');
        if ((rootRhs->type()) == "Binary")
            sideQueue.push('R');

        int level = 0; // level 代表层数
        char side;
        while (!nodeQueue.empty())
        {
            int size = nodeQueue.size(); // 这里一定要使用固定大小size，不使用que.size()，因为que.size是不断变化的
            for (int i = 0; i < size; i++)
            {
                auto node = nodeQueue.front();
                nodeQueue.pop();

                if (level > 0) //第二层开始，nodeQueue每出队一个节点,sideQueue也会出队
                {
                    side = sideQueue.front();
                    sideQueue.pop();
                }

                if (node->getOp() == '/' && level == 0) //根节点为'/'
                {
                    return expr->Clone();
                }

                if (node->getOp() == '*')
                {
                    if (node->getLHS())
                    {
                        if (node->getLHS()->type() == "Binary")
                        {
                            BinaryExprAST *nodeLHSPtr = dynamic_cast<BinaryExprAST *>((node->getLHS()).get());
                            nodeQueue.push(nodeLHSPtr);

                            if (level == 0)
                                side = 'L';
                            if (level > 0)
                                sideQueue.push(side == 'L' ? 'L' : 'R'); //第二层开始，每往队列nodeQueue中添加一个节点，都要在sideQueue中添加一个'L'或者'R'

                            if (nodeLHSPtr->getOp() == '/') //新入队的节点为目标节点
                            {
                                if (side == 'L') //目标节点在根节点的左侧时
                                {
                                    //换操作符
                                    nodeLHSPtr->setOp(root->getOp());
                                    root->setOp('/');
                                    //将根节点和目标节点的左边互换
                                    root->getRHS().swap(nodeLHSPtr->getRHS());

                                    return root->Clone();
                                }
                                if (side == 'R') //目标节点在根节点的右侧时，且 nodeLHSPtr 在node左侧时
                                {
                                    std::unique_ptr<ExprAST> resultRight = nodeLHSPtr->getRHS()->Clone(); //'/'右子树resultRight成为result的右子树
                                    std::unique_ptr<ExprAST> tmp = nodeLHSPtr->getLHS()->Clone();         //'/'左子树根节点将成为替代'/'的节点

                                    node->setLHS(tmp);                                   //改变node的左子树
                                    std::unique_ptr<ExprAST> resultLeft = root->Clone(); // node左子树变化后，root将成为result左子树

                                    return std::make_unique<BinaryExprAST>('/', std::move(resultLeft), std::move(resultRight));
                                }
                            }
                        }
                    }
                    if (node->getRHS())
                    {
                        if (node->getRHS()->type() == "Binary")
                        {
                            BinaryExprAST *nodeRHSPtr = dynamic_cast<BinaryExprAST *>((node->getRHS()).get());
                            nodeQueue.push(nodeRHSPtr);

                            if (level == 0)
                                side = 'R';
                            if (level > 0)
                                sideQueue.push(side == 'L' ? 'L' : 'R');

                            if (nodeRHSPtr->getOp() == '/')
                            {
                                if (side == 'L')
                                {
                                    nodeRHSPtr->setOp(root->getOp());
                                    root->setOp('/');
                                    root->getRHS().swap(nodeRHSPtr->getRHS());
                                    return root->Clone();
                                }
                                if (side == 'R') //目标节点在根节点的右侧时，且 nodeRHSPtr 在node右侧时
                                {
                                    std::unique_ptr<ExprAST> resultRight = nodeRHSPtr->getRHS()->Clone();
                                    std::unique_ptr<ExprAST> tmp = nodeRHSPtr->getLHS()->Clone();

                                    node->setRHS(tmp); //改变node的右子树
                                    std::unique_ptr<ExprAST> resultLeft = root->Clone();

                                    return std::make_unique<BinaryExprAST>('/', std::move(resultLeft), std::move(resultRight));
                                }
                            }
                        }
                    }
                }
            }
            level++;
        }
    }
    return expr->Clone();
}

std::vector<std::unique_ptr<ExprAST>> moveDiv(const std::vector<std::unique_ptr<ExprAST>> &exprs)
{
    // fprintf(stderr, "moveDiv: start--------\n");
    std::vector<std::unique_ptr<ExprAST>> items;
    std::unique_ptr<ExprAST> moveBefore, moveAfter;
    for (long unsigned int i = 0; i < exprs.size(); i++)
    {
        moveBefore = exprs.at(i)->Clone();
        moveAfter = moveDivKernel(moveBefore);
        items.push_back(std::move(moveAfter));
    }
    // print info for debug
    // fprintf(stderr, "\tmoveDiv: expr size = %ld\n", items.size());
    // for (long unsigned int i = 0; i < items.size(); i++)
    // {
    //     fprintf(stderr, "\tmoveDiv: No.%lu: %s\n", i, PrintExpression(items[i]).c_str());
    // }
    // fprintf(stderr, "moveDiv: end----------\n");
    return items;
}

std::unique_ptr<ExprAST> mergeFraction(const std::vector<std::unique_ptr<ExprAST>> &exprs)
{
    //把分子和分母分开
    std::vector<std::unique_ptr<ExprAST>> numerators;
    std::vector<std::unique_ptr<ExprAST>> denominators;
    unsigned int fractionCount = 0;
    for (long unsigned int i = 0; i < exprs.size(); i++)
    {
        std::unique_ptr<ExprAST> exprTmp = exprs.at(i)->Clone();
        std::unique_ptr<ExprAST> numeratorTmp = nullptr;
        std::unique_ptr<ExprAST> denominatorTmp = nullptr;
        if (isFraction(exprTmp))
        {
            numeratorTmp = getNumerator(exprTmp);
            denominatorTmp = getDenominator(exprTmp);
            fractionCount++;
        }
        else
        {
            numeratorTmp = std::move(exprTmp);
            denominatorTmp = std::make_unique<NumberExprAST>(1);
        }
        numerators.push_back(std::move(numeratorTmp));
        denominators.push_back(std::move(denominatorTmp));
    }
    //处理分子，进行循环合并
    std::vector<std::unique_ptr<ExprAST>> numeratorcom;  //建立合并之后的分子数组
    for (long unsigned int i = 0; i < exprs.size(); i++) //写在循环里，每循环一次重置一次
    {
        std::unique_ptr<ExprAST> exprTmp_i = numerators.at(i)->Clone();
        std::unique_ptr<ExprAST> numeratorTmp = nullptr;
        numeratorTmp = std::move(exprTmp_i);
        for (long unsigned int j = 0; j < exprs.size(); j++)
        {
            if (i != j) //乘以除了原本分母的分母
            {
                std::unique_ptr<ExprAST> exprTmp_j = denominators.at(j)->Clone();
                numeratorTmp = mulExpr(numeratorTmp, exprTmp_j);
            }
        }
        numeratorcom.push_back(std::move(numeratorTmp));
    }
    //合并分子
    std::unique_ptr<ExprAST> exprTmp = numeratorcom.at(0)->Clone();
    std::unique_ptr<ExprAST> result = std::move(exprTmp);
    for (long unsigned int i = 1; i < exprs.size(); i++)
    {
        exprTmp = numeratorcom.at(i)->Clone();
        result = addExpr(result, exprTmp);
    }
    //处理分母
    if (fractionCount > 0)
    {
        std::unique_ptr<ExprAST> denominatorTmp = denominators.at(0)->Clone();
        for (long unsigned int i = 1; i < exprs.size(); i++) //循环把各自的分母变成公分母
        {
            exprTmp = denominators.at(i)->Clone();
            denominatorTmp = mulExpr(denominatorTmp, exprTmp);
        }
        result = divExpr(result, denominatorTmp);
    }
    return std::move(result);
}

std::unique_ptr<ExprAST> preprocessInit(const std::unique_ptr<ExprAST> &expr)
{
    std::unique_ptr<ExprAST> exprNew = nullptr;
    if (isFraction(expr))
    {
        exprNew = expr->Clone();
    }
    else
    {
        std::unique_ptr<ExprAST> exprTmp = expandExprWrapper(expr);
        std::vector<std::unique_ptr<ExprAST>> exprs1 = extractItems(exprTmp);
        std::vector<std::unique_ptr<ExprAST>> exprs2 = moveDiv(exprs1);
        exprNew = mergeFraction(exprs2);
    }
    return exprNew;
}

std::unique_ptr<ExprAST> preprocess(const std::unique_ptr<ExprAST> &expr)
{
    std::unique_ptr<ExprAST> exprNew = preprocessInit(expr);

    if (isFraction(exprNew))
    {
        std::unique_ptr<ExprAST> numeratorTmp = getNumerator(exprNew);
        std::unique_ptr<ExprAST> denominatorTmp = getDenominator(exprNew);

        std::unique_ptr<ExprAST> numeratorNew = preprocess(numeratorTmp);
        std::unique_ptr<ExprAST> denominatorNew = preprocess(denominatorTmp);
        return combineFraction(numeratorNew, denominatorNew);
    }
    return std::move(exprNew);
}

struct Monomial;
struct funcInfo
{
    std::string callee;
    std::vector<Monomial> args;

    void showInfo() { fprintf(stderr, "\tfuncInfo: callee = %s, args size = %ld;\n", callee.c_str(), args.size()); }
};

struct variableInfo
{
    std::string name;
    int degree;

    void showInfo() { fprintf(stderr, "\tvariableInfo: name = %s; degree = %d\n", name.c_str(), degree); }
};

struct Monomial
{
    double coefficient = 1;
    std::vector<funcInfo> functions;
    std::vector<variableInfo> variables;

    void showInfo()
    {
        fprintf(stderr, "\tcoefficient = %f;\n", coefficient);
        fprintf(stderr, "\tvariables size = %ld;\n", variables.size());
        for (long unsigned int i = 0; i < variables.size(); i++)
        {
            (variables.at(i)).showInfo();
        }
    }
};

std::vector<variableInfo> mergeVariables(std::vector<variableInfo> vec1, std::vector<variableInfo> vec2)
{
    std::vector<variableInfo> vec3;
    for (long unsigned int i = 0; i < vec1.size(); i++)
    {
        variableInfo &varTmp1 = vec1.at(i);
        variableInfo varTmp3 = varTmp1;

        for (long unsigned int j = 0; j < vec2.size(); j++)
        {
            variableInfo &varTmp2 = vec2.at(j);
            if (varTmp1.name == varTmp2.name)
            {
                varTmp3.degree = varTmp1.degree + varTmp2.degree;
                vec2.erase(vec2.begin() + j);
                break;
            }
        }
        vec3.push_back(varTmp3);
    }
    vec3.insert(vec3.end(), vec2.begin(), vec2.end());

    return vec3;
}

Monomial mergeMonomial(const Monomial &mono1, const Monomial &mono2)
{
    Monomial monoFinal;
    double &coefficient = monoFinal.coefficient;
    std::vector<variableInfo> &variables = monoFinal.variables;
    variableInfo variableTmp;

    coefficient = mono1.coefficient * mono2.coefficient;
    variables = mergeVariables(mono1.variables, mono2.variables);

    return monoFinal;
}

Monomial extractInfoKernel(const std::unique_ptr<ExprAST> &expr)
{
    Monomial monoFinal;
    double &coefficient = monoFinal.coefficient;
    std::vector<variableInfo> &variables = monoFinal.variables;
    variableInfo variableTmp;

    std::string exprType = expr->type();
    std::vector<std::unique_ptr<ExprAST>> items;
    if (exprType == "Number")
    {
        // fprintf(stderr, "extractInfoKernel: number\n");
        NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(expr.get());
        coefficient = numberPtr->getNumber();
        return monoFinal;
    }
    else if (exprType == "Variable")
    {
        // fprintf(stderr, "extractInfoKernel: variable\n");
        VariableExprAST *variablePtr = dynamic_cast<VariableExprAST *>(expr.get());
        variableTmp.name = variablePtr->getVariable();
        variableTmp.degree = 1;
        variables.push_back(variableTmp);
        return monoFinal;
    }
    if (exprType != "Binary")
    {                                                                                        // eg: poly := exp(x) or x or 2.2
        fprintf(stderr, "extractInfoKernel: expr is neither number, variable, or binary\n"); // print info for debug
        return monoFinal;
    }
    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    if (op == '*')
    {
        // fprintf(stderr, "extractInfoKernel: expr op is '*'\n");
        std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
        Monomial monoTmp1 = extractInfoKernel(lhs);
        // monoTmp1.showInfo();
        std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
        Monomial monoTmp2 = extractInfoKernel(rhs);
        // monoTmp2.showInfo();

        monoFinal = mergeMonomial(monoTmp1, monoTmp2);
    }
    else
    {
        fprintf(stderr, "\tERROR: extractInfoKernel: expr contains '+'\n");
    }
    return monoFinal;
}

std::vector<Monomial> extractInfo(const std::vector<std::unique_ptr<ExprAST>> &exprs)
{
    fprintf(stderr, "extractInfo: start--------\n");
    std::vector<Monomial> results;
    for (long unsigned int i = 0; i < exprs.size(); i++)
    {
        std::unique_ptr<ExprAST> exprTmp = exprs.at(i)->Clone();
        Monomial monoTmp = extractInfoKernel(exprTmp);
        results.push_back(monoTmp);
    }
    // print information of info
    for (long unsigned int i = 0; i < results.size(); i++)
    {
        fprintf(stderr, "\tThe Monomial No.%lu: \n", i);
        (results.at(i)).showInfo();
    }
    fprintf(stderr, "extractInfo: end----------\n");
    return results;
}

// TODO: implement
std::vector<struct Monomial> mergePolynomial(const std::vector<struct Monomial> &info)
{
    fprintf(stderr, "mergePolynomial: start--------\n");
    fprintf(stderr, "mergePolynomial: end----------\n");
    return {};
}

// TODO: implement
std::unique_ptr<ExprAST> geneExprAST(std::vector<struct Monomial> &info)
{
    // info为monomial对象容器
    //表达式为数字单项式
    if (info.size() == 1 && info.at(0).variables.size() == 0)
    {
        std::unique_ptr<NumberExprAST> newExpr = std::make_unique<NumberExprAST>(NumberExprAST(info.at(0).coefficient));
        return newExpr.get()->Clone();
    }
    //表达式为变量单项式
    if (info.size() == 1 && info.at(0).variables.size() == 1 && info.at(0).variables.at(0).degree == 1)
    {
        std::unique_ptr<VariableExprAST> newExpr = std::make_unique<VariableExprAST>(VariableExprAST(info.at(0).variables.at(0).name));
        return newExpr.get()->Clone();
    }

    std::unique_ptr<BinaryExprAST> newExpr = std::make_unique<BinaryExprAST>(BinaryExprAST('+', nullptr, nullptr));
    std::unique_ptr<BinaryExprAST> tempBinaryAST = nullptr;
    std::unique_ptr<BinaryExprAST> binaryASTLhs = nullptr;
    std::unique_ptr<ExprAST> ExprASTASTVar = nullptr;
    std::unique_ptr<ExprAST> tempBinaryASTLhs = nullptr;
    //先创建一个临时的binaryAST,将第一个表达式作为左子树
    for (int i = 0; i < info.size(); i++)
    {

        if (i == 0)
        {
            //表达式为数字单项式
            if (info.at(i).variables.size() == 0)
            {
                tempBinaryASTLhs = std::move(std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient)));
            }
            //表达式为变量单项式
            else if ((info.at(i).variables.size() == 1) && (info.at(i).variables.at(0).degree == 1))
            {
                tempBinaryASTLhs = std::move(std::make_unique<VariableExprAST>(VariableExprAST(info.at(i).variables.at(0).name)));
            }
            else
            {
                //该容器存放变量名
                std::vector<std::string> varVec;
                std::cout << "a" << std::endl;
                //将变量名存入容器
                for (int m = 0; m < info.at(i).variables.size(); m++)
                {
                    for (int n = 0; n < info.at(i).variables.at(m).degree; n++)
                    {
                        varVec.push_back(info.at(i).variables.at(m).name);
                    }
                }
                std::cout << "b" << std::endl;
                //对容器排序
                std::sort(varVec.begin(), varVec.end());

                //系数不等于1
                if (info.at(i).coefficient != 1)
                {

                    std::unique_ptr<NumberExprAST> numAST = std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient));

                    binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(numAST), nullptr)));

                    for (int v = 0; v < varVec.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        binaryASTLhs.get()->setRHS(varAST1);

                        if (v == varVec.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = binaryASTLhs.get()->Clone();
                        binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
                else //系数等于1
                {

                    std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec.at(0)));

                    binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(varAST), nullptr)));
                    for (int v = 1; v < varVec.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        binaryASTLhs.get()->setRHS(varAST1);

                        if (v == varVec.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = binaryASTLhs.get()->Clone();
                        binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
                tempBinaryASTLhs = binaryASTLhs->Clone();
            }
        }
        //表达式为变量单项式
        if ((info.at(i).variables.size() == 1) && (info.at(i).variables.at(0).degree == 1) && i > 0)
        {
            ExprASTASTVar = std::move(std::make_unique<VariableExprAST>(VariableExprAST(info.at(i).variables.at(0).name)));
        }

        else if (i > 0)
        {
            std::cout << "5" << std::endl;
            //该容器存放变量名
            std::vector<std::string> varVec;
            std::cout << "a" << std::endl;
            //将变量名存入容器
            for (int m = 0; m < info.at(i).variables.size(); m++)
            {
                for (int n = 0; n < info.at(i).variables.at(m).degree; n++)
                {
                    varVec.push_back(info.at(i).variables.at(m).name);
                }
            }
            std::cout << "b" << std::endl;
            //对容器排序
            std::sort(varVec.begin(), varVec.end());

            //系数不等于1
            if (info.at(i).coefficient != 1)
            {

                std::unique_ptr<NumberExprAST> numAST = std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient));

                tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(numAST), nullptr)));

                for (int v = 0; v < varVec.size(); v++)
                {
                    std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec.at(v)));
                    std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                    tempBinaryAST.get()->setRHS(varAST1);

                    if (v == varVec.size() - 1)
                        break;
                    std::unique_ptr<ExprAST> tempBinaryAST3 = tempBinaryAST.get()->Clone();
                    tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                }
            }
            else //系数等于1
            {

                std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec.at(0)));

                tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(varAST), nullptr)));
                for (int v = 1; v < varVec.size(); v++)
                {
                    std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec.at(v)));
                    std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                    tempBinaryAST.get()->setRHS(varAST1);

                    if (v == varVec.size() - 1)
                        break;
                    std::unique_ptr<ExprAST> tempBinaryAST3 = tempBinaryAST.get()->Clone();
                    tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                }
            }
        }
        if (i == 0)
        {
            std::unique_ptr<ExprAST> tempBinaryAST1 = tempBinaryASTLhs.get()->Clone();
            newExpr->setLHS(tempBinaryAST1);
            continue;
        }
        else
        {
            std::unique_ptr<ExprAST> tempBinaryAST2 = nullptr;
            if ((info.at(i).variables.size() == 1) && (info.at(i).variables.at(0).degree == 1))
            {
                tempBinaryAST2 = std::move(ExprASTASTVar.get()->Clone());
            }
            else
            {
                tempBinaryAST2 = std::move(tempBinaryAST.get()->Clone());
            }
            newExpr->setRHS(tempBinaryAST2);
        }
        if (i != (info.size() - 1))
        {
            std::unique_ptr<ExprAST> newExpr1 = newExpr.get()->Clone();
            newExpr = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('+', std::move(newExpr1), nullptr)));
        }
    }

    return newExpr->Clone();
}

// TODO: implement. rewriteExpr should contain poly and math function equal change
std::vector<std::unique_ptr<ExprAST>> rewriteExpr(const std::unique_ptr<ExprAST> &expr)
{
    std::vector<std::unique_ptr<ExprAST>> results;
    return results;
}

// TODO: check
std::vector<std::unique_ptr<ExprAST>> rewriteExprWrapper(std::unique_ptr<ExprAST> &expr)
{
    std::vector<std::unique_ptr<ExprAST>> items = extractItems(expr);
    std::vector<Monomial> info = extractInfo(items);
    std::vector<Monomial> infoNew = mergePolynomial(info);
    std::unique_ptr<ExprAST> exprNew = geneExprAST(infoNew);
    return rewriteExpr(exprNew);
}

// TODO: implement
std::vector<std::unique_ptr<ExprAST>> createAll(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators)
{
    std::vector<std::unique_ptr<ExprAST>> results;
    return results;
}

// TODO: check
std::vector<std::unique_ptr<ExprAST>> exprAuto(std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "exprAuto: ERROR: the input expr is nullptr!\n");
        return {};
    }
    fprintf(stderr, "exprAuto: step1: preprocess\n");
    fprintf(stderr, "exprAuto: expr = %s\n", PrintExpression(expr).c_str());
    std::unique_ptr<ExprAST> exprNew = preprocess(expr);
    fprintf(stderr, "exprAuto: exprNew = %s\n", PrintExpression(exprNew).c_str());

    std::vector<std::unique_ptr<ExprAST>> exprsFinal;
    fprintf(stderr, "exprAuto: step2: judge if exprNew is a fraction\n");
    if (isFraction(exprNew))
    {
        fprintf(stderr, "exprAuto: exprNew is a fraction, so perform step3 and step4\n");
        std::unique_ptr<ExprAST> numeratorTmp = getNumerator(exprNew);
        std::unique_ptr<ExprAST> denominatorTmp = getDenominator(exprNew);

        auto numeratorsFinal = rewriteExprWrapper(numeratorTmp);
        auto denominatorsFinal = rewriteExprWrapper(denominatorTmp);
        exprsFinal = createAll(numeratorsFinal, denominatorsFinal);
    }
    else
    {
        fprintf(stderr, "exprAuto: exprNew is not a fraction, so perform step4\n");
        exprsFinal = rewriteExprWrapper(exprNew);
    }

    fprintf(stderr, "exprAuto: exprsFinal size = %ld\n", exprsFinal.size());
    for (long unsigned int i = 0; i < exprsFinal.size(); i++)
    {
        fprintf(stderr, "exprAuto: No.%lu: %s\n", i, PrintExpression(exprsFinal[i]).c_str());
    }
    return exprsFinal;
}
