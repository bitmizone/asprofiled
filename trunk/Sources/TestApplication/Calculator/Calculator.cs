using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestApplication.Calculator
{
    class Test
    {
        
    }

    class  Calculator : Object
    {
        private int aa = 2;
        private TT abba = new TT();
        [AsContract("c.abba.baba == 31", "^c.abba.baba == 31", null)]
        public int Div(bool t, int divided, int divisor, string val, string delta, Calculator c)
        {
            if (divided > 0) 
                Div(t, --divided, divisor,val, delta,c);
            val = "barabara";
            c.abba.baba = 3;
            return 0;
        }

        [AsContract("", "@returnValue.abba.baba == 31 && @returnValue.aa == 2", null)]
        public Calculator TestMe()
        {
            Calculator c = new Calculator();
            c.abba.baba = 10;
            return c;
        }
    }

    class TT
    {
        public int baba = 31;
    }

    [AttributeUsage(AttributeTargets.All, AllowMultiple = true)]
    public class AsContractAttribute : Attribute
    {

        public AsContractAttribute()
        {
        }

        public AsContractAttribute(string preCondition, string invariant, string postCondition)
        {

        }

        public string PostCondition
        {
            get;
            set;
        }

        public string Invariant
        {
            get;
            set;
        }

        public string PreCondition
        {
            get;
            set;

        }
    }

  
}
