using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TestApplication.Calculator;
using LinFu.DesignByContract2.Contracts;

namespace TestApplication
{
    class Program
    {
        static void Main(string[] args)
        {
            Calculator.Calculator calculatorInstance = new TestApplication.Calculator.Calculator();
            calculatorInstance.Div(false, 2, 4096, "baba", "3", new Calculator.Calculator());
            TestFalse(false);
            TestTrue(true);
            Random r = new Random();
            for (int i = 0; i < 100; i++)
            {
                //Test(r.Next() % 2 == 0);
            }

            MsContractsCalculator cc = new MsContractsCalculator();
            cc.Div(DateTime.Now.Second, 2);

            AdHocContract contract = new AdHocContract();
            Require.On(contract).ForMethodWith(t => t.Name == "Mult").That<int>(arg => arg != 0).OtherwisePrint("Error");
            
        }

        static void Test(bool val)
        {
        }

        static void TestTrue(bool valTrue)
        {
        }

        static void TestFalse(bool valFalse)
        {
        }
    }
}
