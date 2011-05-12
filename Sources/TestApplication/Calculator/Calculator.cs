using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics.Contracts;
using LinFu.DesignByContract2.Contracts;

namespace TestApplication.Calculator
{

    class  Calculator : Object
    {
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

    class MsContractsCalculator
    {
        [AsContract("arg2 != 0", null, null)]
        public int Div(int arg1, int arg2)
        {
            Contract.Requires(arg2 != 0);
            Contract.Ensures(Contract.Result<int>() >= 0 && Contract.OldValue(arg1) == 1);
            return arg1 / arg2;
        }

        public void ObjectTest(Test t)
        {
            
            Contract.Ensures(Contract.OldValue<Test>(t).member + 4 == t.member);
            t.member += 4;
        }

        public int Factorial(int n)
        {
            Contract.Ensures(Contract.Result<int>() == n * Factorial(n - 1));
            if (n == 0)
                return 1;
            return n * Factorial(n - 1);
        }

    }

    class LinFuCalculator
    {
        
        public int Mult(int arg1, int arg2)
        {
            
            return 0;
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
