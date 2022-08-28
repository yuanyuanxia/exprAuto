#include <algorithm>
#include <queue>

#include "basic.hpp"
#include "monoInfo.hpp"

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
    return result;
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
    return exprNew;
}

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

monoInfo mergeMonomial(const monoInfo &mono1, const monoInfo &mono2)
{
    monoInfo monoFinal;
    double &coefficient = monoFinal.coefficient;
    std::vector<funcInfo> &functions = monoFinal.functions;
    std::vector<variableInfo> &variables = monoFinal.variables;
    variableInfo variableTmp;

    coefficient = mono1.coefficient * mono2.coefficient;
    variables = mergeVariables(mono1.variables, mono2.variables);

    for(size_t i = 0; i < (mono1.functions).size(); ++i)
    {
        auto arg = (mono1.functions).at(i);
        functions.push_back(std::move(arg));
    }
    for(size_t i = 0; i < (mono2.functions).size(); ++i)
    {
        auto arg = (mono2.functions).at(i);
        functions.push_back(std::move(arg));
    }

    return monoFinal;
}

std::vector<monoInfo> extractInfo(const std::vector<std::unique_ptr<ExprAST>> &exprs);

monoInfo extractInfoKernel(const std::unique_ptr<ExprAST> &expr)
{
    monoInfo monoFinal;
    double &coefficient = monoFinal.coefficient;
    std::vector<funcInfo> &functions = monoFinal.functions;
    std::vector<variableInfo> &variables = monoFinal.variables;
    funcInfo funcTmp;
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
    else if (exprType == "Call")
    {
        // fprintf(stderr, "extractInfoKernel: variable\n");
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        funcTmp.callee = callPtr->getCallee();
        for(size_t i = 0; i < (callPtr->getArgs()).size(); ++i)
        {
            auto arg = (callPtr->getArgs().at(i))->Clone();
            auto exprsTmp = extractItems(arg);
            auto funcs = extractInfo(exprsTmp);
            (funcTmp.args).insert((funcTmp.args).begin(), funcs.begin(), funcs.end());
        }
        functions.push_back(funcTmp);
        return monoFinal;
    }
    if (exprType != "Binary")
    {   // eg: poly := exp(x) or x or 2.2
        fprintf(stderr, "extractInfoKernel: expr is neither number, variable, call or binary\n");
        return monoFinal;
    }
    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    if (op == '*')
    {
        // fprintf(stderr, "extractInfoKernel: expr op is '*'\n");
        std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
        monoInfo monoTmp1 = extractInfoKernel(lhs);
        std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
        monoInfo monoTmp2 = extractInfoKernel(rhs);

        monoFinal = mergeMonomial(monoTmp1, monoTmp2);
    }
    else
    {
        fprintf(stderr, "\tERROR: extractInfoKernel: expr contains '+'\n");
    }
    return monoFinal;
}

std::vector<monoInfo> extractInfo(const std::vector<std::unique_ptr<ExprAST>> &exprs)
{
    fprintf(stderr, "extractInfo: start--------\n");
    std::vector<monoInfo> results;
    for (long unsigned int i = 0; i < exprs.size(); i++)
    {
        std::unique_ptr<ExprAST> exprTmp = exprs.at(i)->Clone();
        monoInfo monoTmp = extractInfoKernel(exprTmp);
        std::vector<variableInfo> &variables = monoTmp.variables;
        std::sort(variables.begin(), variables.end());
        results.push_back(monoTmp);
    }

    // print information of info
    // for (long unsigned int i = 0; i < results.size(); i++)
    // {
    //     fprintf(stderr, "The monoInfo No.%lu: \n", i);
    //     (results.at(i)).showInfo();
    //     fprintf(stderr, "\n");
    // }
    fprintf(stderr, "extractInfo: end----------\n");
    return results;
}

std::vector<monoInfo> mergePolynomial(const std::vector<monoInfo> &info)
{
    fprintf(stderr, "mergePolynomial: start--------\n");
    size_t size = info.size();
    bool *merged = new bool[size]{};
    size_t i = 0, j = 0;
    std::vector<monoInfo> results;
    while (i < info.size())
    {
        if(merged[i])
        {
            i++;
            continue;
        }
        monoInfo tmp = info.at(i);
        merged[i] = true;
        j = i + 1;
        while (j < info.size())
        {
            monoInfo tmp1 = info.at(j);
            if(tmp.hasCommonTerm(tmp1))
            {
                tmp.combine(tmp1);
                merged[j] = true;
            }
            j++;
        }
        results.push_back(tmp);
    }
    std::sort(results.begin(), results.end());
    // print information of info
    // for (size_t i = 0; i < results.size(); i++)
    // {
    //     fprintf(stderr, "The Monomial No.%lu: \n", i);
    //     (results.at(i)).showInfo();
    //     fprintf(stderr, "\n");
    // }
    delete []merged;
    fprintf(stderr, "mergePolynomial: end----------\n");
    return results;
}

std::unique_ptr<ExprAST> geneExprAST(std::vector<monoInfo> &info)
{
    fprintf(stderr, "geneExprAST: start--------\n");
    // info为monomial对象容器
    //表达式为数字单项式
    if (info.size() == 1 && info.at(0).variables.size() == 0)
    {
        std::unique_ptr<NumberExprAST> newExpr = std::make_unique<NumberExprAST>(NumberExprAST(info.at(0).coefficient));
        fprintf(stderr, "geneExprAST: end--------\n");
        return newExpr.get()->Clone();
    }
    //表达式为变量单项式
    if (info.size() == 1 && info.at(0).variables.size() == 1 && info.at(0).variables.at(0).degree == 1 && info.at(0).coefficient == 1)
    {
        std::unique_ptr<VariableExprAST> newExpr = std::make_unique<VariableExprAST>(VariableExprAST(info.at(0).variables.at(0).name));
        fprintf(stderr, "geneExprAST: end--------\n");
        return newExpr.get()->Clone();
    }

    std::unique_ptr<BinaryExprAST> newExpr = std::make_unique<BinaryExprAST>(BinaryExprAST('+', nullptr, nullptr));
    std::unique_ptr<BinaryExprAST> tempBinaryAST = nullptr;
    std::unique_ptr<BinaryExprAST> binaryASTLhs = nullptr;
    std::unique_ptr<ExprAST> ExprASTASTVar = nullptr;
    std::unique_ptr<ExprAST> tempBinaryASTLhs = nullptr;
    //先创建一个临时的binaryAST,将第一个表达式作为左子树
    for (size_t i = 0; i < info.size(); i++)
    {
        if (i == 0)
        {
            //表达式为数字单项式
            if (info.at(i).variables.size() == 0)
            {
                tempBinaryASTLhs = std::move(std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient)));
            }
            //表达式为变量单项式
            else if ((info.at(i).variables.size() == 1) && (info.at(i).coefficient == 1) && (info.at(i).variables.at(0).degree == 1))
            {
                tempBinaryASTLhs = std::move(std::make_unique<VariableExprAST>(VariableExprAST(info.at(i).variables.at(0).name)));
            }
            else
            {
                //该容器存放变量名
                std::vector<std::string> varVec1;

                //将变量名存入容器
                for (size_t m = 0; m < info.at(i).variables.size(); m++)
                {
                    for (int n = 0; n < info.at(i).variables.at(m).degree; n++)
                    {
                        varVec1.push_back(info.at(i).variables.at(m).name);
                    }
                }

                //对容器排序
                std::sort(varVec1.begin(), varVec1.end());
                // std::cout << "begin to print vector" << std::endl;
                // for (int k = 0; k < varVec1.size(); k++)
                // {
                //     std::cout << varVec1.at(k) << std::endl;
                // }
                // std::cout << "print end" << std::endl;

                //系数不等于1
                if (info.at(i).coefficient != 1)
                {
                    // std::cout << "coefficient:" << info.at(i).coefficient << std::endl;
                    std::unique_ptr<NumberExprAST> numAST = std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient));

                    binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(numAST), nullptr)));

                    for (size_t v = 0; v < varVec1.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec1.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        binaryASTLhs.get()->setRHS(varAST1);

                        if (v == varVec1.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = binaryASTLhs.get()->Clone();
                        binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                    if (info.size() == 1)
                    {
                        fprintf(stderr, "geneExprAST: end--------\n");
                        return binaryASTLhs->Clone();
                    }
                }
                else //系数等于1
                {
                    std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec1.at(0)));

                    binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(varAST), nullptr)));
                    for (size_t v = 1; v < varVec1.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec1.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        binaryASTLhs.get()->setRHS(varAST1);

                        if (v == varVec1.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = binaryASTLhs.get()->Clone();
                        binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
                tempBinaryASTLhs = binaryASTLhs->Clone();
            }
        }
        else
        {
            // std::cout << "here" << std::endl;

            //表达式为变量单项式
            if ((info.at(i).variables.size() == 1) && (info.at(i).variables.at(0).degree == 1) && i > 0 && info.at(i).coefficient == 1)
            {
                ExprASTASTVar = std::move(std::make_unique<VariableExprAST>(VariableExprAST(info.at(i).variables.at(0).name)));
            }
            else
            {
                //该容器存放变量名
                std::vector<std::string> varVec2;
                //将变量名存入容器
                for (size_t m = 0; m < info.at(i).variables.size(); m++)
                {
                    for (int n = 0; n < info.at(i).variables.at(m).degree; n++)
                    {
                        varVec2.push_back(info.at(i).variables.at(m).name);
                    }
                }

                //对容器排序
                std::sort(varVec2.begin(), varVec2.end());

                // std::cout << "begin to print vector" << std::endl;
                // for (int k = 0; k < varVec2.size(); k++)
                // {
                //     std::cout << varVec2.at(k) << std::endl;
                // }

                //系数不等于1
                if (info.at(i).coefficient != 1)
                {
                    // std::cout << "coefficient:" << info.at(i).coefficient << std::endl;

                    std::unique_ptr<NumberExprAST> numAST = std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient));

                    tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(numAST), nullptr)));

                    for (size_t v = 0; v < varVec2.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec2.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        tempBinaryAST.get()->setRHS(varAST1);

                        if (v == varVec2.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = tempBinaryAST.get()->Clone();
                        tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
                else //系数等于1
                {
                    std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec2.at(0)));

                    tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(varAST), nullptr)));
                    for (size_t v = 1; v < varVec2.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec2.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        tempBinaryAST.get()->setRHS(varAST1);

                        if (v == varVec2.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = tempBinaryAST.get()->Clone();
                        tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
            }
        }

        if (i == 0)
        {
            std::unique_ptr<ExprAST> tempBinaryAST1 = tempBinaryASTLhs.get()->Clone();
            newExpr->setLHS(tempBinaryAST1);
            if (info.size() == 1)
            {
                fprintf(stderr, "geneExprAST: end--------\n");
                return tempBinaryAST1->Clone();
            }
            continue;
        }
        else
        {
            std::unique_ptr<ExprAST> tempBinaryAST2 = nullptr;
            if ((info.at(i).variables.size() == 1) && (info.at(i).variables.at(0).degree == 1) && (info.at(i).coefficient == 1))
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
    fprintf(stderr, "geneExprAST: end--------\n");
    return newExpr->Clone();
}

// TODO: implement. rewriteExpr should contain poly and math function equal change
std::vector<std::unique_ptr<ExprAST>> rewriteExpr(const std::unique_ptr<ExprAST> &expr)
{
    fprintf(stderr, "rewriteExpr: start--------\n");
    std::vector<std::unique_ptr<ExprAST>> results;
    results.push_back(std::move(expr->Clone()));
    fprintf(stderr, "rewriteExpr: end--------\n");
    return results;
}

// TODO: check
std::vector<std::unique_ptr<ExprAST>> rewriteExprWrapper(std::unique_ptr<ExprAST> &expr)
{
    std::vector<std::unique_ptr<ExprAST>> items = extractItems(expr);
    std::vector<monoInfo> info = extractInfo(items);
    std::vector<monoInfo> infoNew = mergePolynomial(info);
    std::unique_ptr<ExprAST> exprNew = geneExprAST(infoNew);
    return rewriteExpr(exprNew);
}

std::vector<std::unique_ptr<ExprAST>> createAll(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators)
{
    std::unique_ptr<ExprAST> resultsTemp;
    std::vector<std::unique_ptr<ExprAST>> results;

    if(numerators.at(0) != nullptr && denominators.at(0) != nullptr)
    {
        for(long unsigned int i = 0; i < numerators.size(); i++)
        {
            for(long unsigned int j = 0; j < denominators.size(); j++)
            {
                resultsTemp = divExpr(numerators.at(i), denominators.at(j));
                results.push_back(std::move(resultsTemp));
            }
        }
        return results;
    }
    else
    {
        fprintf(stderr, "\tERROR: creatAll: the input include nullptr!\n");
        return {};
    }
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
