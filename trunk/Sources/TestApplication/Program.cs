using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TestApplication.Calculator;

namespace TestApplication
{
    class Program
    {
        static void Main(string[] args)
        {
            Calculator.Calculator calculatorInstance = new TestApplication.Calculator.Calculator();
            calculatorInstance.Div(false, 2, 4096, "barnaba > 5 habla bhable bhable bhable bhable bhable bhable ");
            TestFalse(false);
            TestTrue(true);
            Random r = new Random();
            for (int i = 0; i < 100; i++)
            {
                //Test(r.Next() % 2 == 0);
            }
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
