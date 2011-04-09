using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestApplication.Calculator
{

    class  Calculator : Object
    {
        private int aa = 2;
        private Test test = new Test();
        [AsContract("c.test.member == 31 && divided > 1 || val == \"test\"", "divided / divisor > 0 && @returnValue == 0 || val == \"test\"",null)]
        public int Div(bool t, int divided, int divisor, string val, string delta, Calculator c)
        {
            if (divided > 0) 
                //Div(t, --divided, divisor,val, delta,c);
            val = "barabara";
            c.test.member = 3;
            return 0;
        }

        [AsContract("", "@returnValue.abba.baba == 31 && @returnValue.aa == 2",null)]
        public Calculator TestMe()
        {
            Calculator c = new Calculator();
            c.test.member = 10;
            return c;
        }
    }

    class Test
    {
        public int member = 31;
    }

    [AttributeUsage(AttributeTargets.All, AllowMultiple = false)]
    public class AsContractAttribute : Attribute
    {

        public AsContractAttribute(string preCondition, string postCondition,string t)
        {

        }
    }

  
}
