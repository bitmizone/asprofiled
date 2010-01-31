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
            int a = 999;
            calculatorInstance.Div(true,0, 4096, "barnaba > 4");
        }
    }
}
