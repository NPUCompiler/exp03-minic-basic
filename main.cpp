/**
 * @file main.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 主程序文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <string>
#include <getopt.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Common.h"
#include "AST.h"
#include "Antlr4Executor.h"
#include "CodeGenerator.h"
#include "CodeGeneratorArm32.h"
#include "FlexBisonExecutor.h"
#include "FrontEndExecutor.h"
#include "Graph.h"
#include "IRGenerator.h"
#include "RecursiveDescentExecutor.h"
#include "Module.h"

///
/// @brief 是否显示帮助信息
///
static bool gShowHelp = false;

///
/// @brief 显示抽象语法树，非线性IR
///
static bool gShowAST = false;

///
/// @brief 产生线性IR，线性IR，默认输出
///
static bool gShowLineIR = false;

///
/// @brief 显示汇编
///
static bool gShowASM = false;

///
/// @brief 输出中间IR，含汇编或者自定义IR等，默认输出线性IR
///
static bool gShowSymbol = false;

///
/// @brief 前端分析器，默认选Flex和Bison
///
static bool gFrontEndFlexBison = true;

///
/// @brief 前端分析器Antlr4，是否选中
///
static bool gFrontEndAntlr4 = false;

///
/// @brief 前端分析器用递归下降分析法，是否选中
///
static bool gFrontEndRecursiveDescentParsing = false;

///
/// @brief 在输出汇编时是否输出中间IR作为注释
///
static bool gAsmAlsoShowIR = false;

/// @brief 优化的级别，即-O后面的数字，默认为0
static int gOptLevel = 0;

/// @brief 指定CPU目标架构，这里默认为ARM32
static std::string gCPUTarget = "ARM32";

/// @brief 输入源文件
static std::string gInputFile;

/// @brief 输出文件，不同的选项输出的内容不同
static std::string gOutputFile;

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"output", required_argument, 0, 'o'},
    {"symbol", no_argument, 0, 'S'},
    {"ast", no_argument, 0, 'T'},
    {"ir", no_argument, 0, 'I'},
    {"antlr4", no_argument, 0, 'A'},
    {"recursive-descent", no_argument, 0, 'D'},
    {"optimize", required_argument, 0, 'O'},
    {"target", required_argument, 0, 't'},
    {"asmir", no_argument, 0, 'c'},
    {0, 0, 0, 0}
};

/// @brief 显示帮助
/// @param exeName
static void showHelp(const std::string & exeName)
{
    std::cout << exeName + " -S [--symbol] [-A | --antlr4 | -D | --recursive-descent] [-T | --ast | -I | --ir] [-o output | --output=output] source\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help                 Show this help message\n";
    std::cout << "  -o, --output=FILE          Specify output file\n";
    std::cout << "  -S, --symbol               Show symbol information\n";
    std::cout << "  -T, --ast                  Output abstract syntax tree\n";
    std::cout << "  -I, --ir                   Output intermediate representation\n";
    std::cout << "  -A, --antlr4               Use Antlr4 for lexical and syntax analysis\n";
    std::cout << "  -D, --recursive-descent    Use recursive descent parsing\n";
    std::cout << "  -O, --optimize=LEVEL       Set optimization level\n";
    std::cout << "  -t, --target=CPU           Specify target CPU architecture\n";
    std::cout << "  -c, --asmir                Show IR instructions as comments in assembly output\n";
}

/// @brief 参数解析与有效性检查
/// @param argc
/// @param argv
/// @return
static int ArgsAnalysis(int argc, char * argv[])
{
    int ch;

    // 指定参数解析的选项，可识别-h、-o、-S、-T、-I、-A、-D等选项
    // -S必须项，输出中间IR、抽象语法树或汇编
    // -T指定时输出AST，-I输出中间IR，不指定则默认输出汇编
    // -A指定按照antlr4进行词法与语法分析，-D指定按照递归下降分析法执行，不指定时按flex+bison执行
    // -o要求必须带有附加参数，指定输出的文件
    // -O要求必须带有附加整数，指明优化的级别
    // -t要求必须带有目标CPU，指明目标CPU的汇编
    // -c选项在输出汇编时有效，附带输出IR指令内容
    const char options[] = "ho:STIADO:t:c";
    int option_index = 0;

    opterr = 1;

lb_check:
    while ((ch = getopt_long(argc, argv, options, long_options, &option_index)) != -1) {
        switch (ch) {
            case 'h':
                gShowHelp = true;
                break;
            case 'o':
                gOutputFile = optarg;
                break;
            case 'S':
                gShowSymbol = true;
                break;
            case 'T':
                gShowAST = true;
                break;
            case 'I':
                // 产生中间IR
                gShowLineIR = true;
                break;
                break;
            case 'A':
                // 选用antlr4
                gFrontEndAntlr4 = true;
                gFrontEndFlexBison = false;
                gFrontEndRecursiveDescentParsing = false;
                break;
            case 'D':
                // 选用递归下降分析法与词法手动实现
                gFrontEndAntlr4 = false;
                gFrontEndFlexBison = false;
                gFrontEndRecursiveDescentParsing = true;
                break;
            case 'O':
                // 优化级别分析，暂时没有用，如开启优化时请使用
                gOptLevel = std::stoi(optarg);
                break;
            case 't':
                gCPUTarget = optarg;
                break;
            case 'c':
                gAsmAlsoShowIR = true;
                break;
            default:
                return -1;
                break; /* no break */
        }
    }

    argc -= optind;
    argv += optind;

    if (argc >= 1) {

        // 第一次设置
        if (gInputFile.empty()) {

            gInputFile = argv[0];
        } else {
            // 重复设置则出错
            return -1;
        }

        if (argc > 1) {
            // 多余一个参数，则说明输入的源文件后仍然有参数要解析
            optind = 0;
            goto lb_check;
        }
    }

    // 必须指定要进行编译的输入文件
    if (gInputFile.empty()) {
        return -1;
    }

    // 显示符号信息，必须指定，可选抽象语法树、中间IR(DragonIR)等显示
    if (!gShowSymbol) {
        return -1;
    }

    int flag = (int) gShowLineIR + (int) gShowAST;

    if (0 == flag) {
        // 没有指定，则输出汇编指令
        gShowASM = true;
    } else if (flag != 1) {
        // 线性中间IR、抽象语法树只能同时选择一个
        return -1;
    }

    // 没有指定输出文件则产生默认文件
    if (gOutputFile.empty()) {

        // 默认文件名
        if (gShowAST) {
            gOutputFile = "output.png";
        } else if (gShowLineIR) {
            gOutputFile = "output.ir";
        } else {
            gOutputFile = "output.s";
        }
    }

    return 0;
}

///
/// @brief 对源文件进行编译处理生成汇编
/// @return true 成功
/// @return false 失败
///
static int compile(std::string inputFile, std::string outputFile)
{
    // 函数返回值，默认-1
    int result = -1;

    // 内部函数调用返回值保存变量
    int subResult;

    Module * module = nullptr;

    // 这里采用do {} while(0)架构的目的是如果处理出错可通过break退出循环，出口唯一
    // 在编译器编译优化时会自动去除，因为while恒假的缘故
    do {

        // 编译过程主要包括：
        // 1）词法语法分析生成AST
        // 2) 遍历AST生成线性IR
        // 3) 对线性IR进行优化：目前不支持
        // 4) 把线性IR转换成汇编

        // 创建词法语法分析器
        FrontEndExecutor * frontEndExecutor;
        if (gFrontEndAntlr4) {
            // Antlr4
            frontEndExecutor = new Antlr4Executor(inputFile);
        } else if (gFrontEndRecursiveDescentParsing) {
            // 递归下降分析法
            frontEndExecutor = new RecursiveDescentExecutor(inputFile);
        } else {
            // 默认为Flex+Bison
            frontEndExecutor = new FlexBisonExecutor(inputFile);
        }

        // 前端执行：词法分析、语法分析后产生抽象语法树，其root为全局变量ast_root
        subResult = frontEndExecutor->run();
        if (!subResult) {

            minic_log(LOG_ERROR, "前端分析错误");
            // 退出循环
            break;
        }

        // 获取抽象语法树的根节点
        ast_node * astRoot = frontEndExecutor->getASTRoot();

        // 清理前端资源
        delete frontEndExecutor;

        // 这里可进行非线性AST的优化

        if (gShowAST) {

            // 遍历抽象语法树，生成抽象语法树图片
            OutputAST(astRoot, outputFile);

            // 清理抽象语法树
            free_ast(astRoot);

            // 设置返回结果：正常
            result = 0;

            break;
        }

        // 输出线性中间IR、计算器模拟解释执行、输出汇编指令
        // 都需要遍历AST转换成线性IR指令

        // 符号表，保存所有的变量以及函数等信息
        Module * module = new Module(inputFile);

        // 遍历抽象语法树产生线性IR，相关信息保存到符号表中
        IRGenerator ast2IR(astRoot, module);
        subResult = ast2IR.run();
        if (!subResult) {

            // 输出错误信息
            minic_log(LOG_ERROR, "中间IR生成错误");

            break;
        }

        // 清理抽象语法树
        free_ast(astRoot);

        if (gShowLineIR) {

            // 对IR的名字重命名
            module->renameIR();

            // 输出IR
            module->outputIR(outputFile);

            // 设置返回结果：正常
            result = 0;

            break;
        }

        // 要使得汇编能输出IR指令作为注释，必须对IR的名字进行命名，否则为空值
        if (gAsmAlsoShowIR) {
            // 对IR的名字重命名
            module->renameIR();
        }

        // 这里可追加中间代码优化，体系结果无关的优化等

        // 后端处理，体系结果相关的操作
        // 这里提供一种面向ARM32的汇编产生器CodeGeneratorArm32作为参考
        // 需要时可根据需要修改或追加新的目标体系架构
        if (gShowASM) {

            CodeGenerator * generator = nullptr;

            if (gCPUTarget == "ARM32") {
                // 输出面向ARM32的汇编指令
                generator = new CodeGeneratorArm32(module);
                generator->setShowLinearIR(gAsmAlsoShowIR);
                generator->run(outputFile);
            } else {
                // 不支持指定的CPU架构
                minic_log(LOG_ERROR, "指定的目标CPU架构(%s)不支持", gCPUTarget.c_str());
                break;
            }

            delete generator;
        }

        // 清理符号表
        module->Delete();

        // 成功执行
        result = 0;

    } while (false);

    delete module;

    return result;
}

/// @brief 主程序
/// @param argc
/// @param argv
/// @return
int main(int argc, char * argv[])
{
    // 函数返回值，默认-1
    int result = -1;

#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif

    // 参数解析
    result = ArgsAnalysis(argc, argv);
    if (result < 0) {

        // 在终端显示程序帮助信息
        showHelp(argv[0]);

        return -1;
    }

    // 显示帮助
    if (gShowHelp) {

        // 在终端显示程序帮助信息
        showHelp(argv[0]);

        return 0;
    }

    // 参数解析正确，进行编译处理，目前只支持一个文件的编译。
    result = compile(gInputFile, gOutputFile);

    return result;
}
