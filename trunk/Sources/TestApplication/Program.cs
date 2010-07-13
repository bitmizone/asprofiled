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
            calculatorInstance.Div(true,2, 4096, "barnaba > 4");
        }
    }
}
