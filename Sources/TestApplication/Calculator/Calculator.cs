using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics.Contracts;
using LinFu.DesignByContract2.Contracts;
using LinFu.DesignByContract2.Core;

namespace TestApplication.Calculator
{

    class Calculator : Object
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


    class AccountAP
    {
        public int Balance;
    }

    class AccountManager
    {
        [AsContract("source.Balance > amount",
            "@returnValue == amount", null)]// && ^source.Balance + ^destination.Balance == source.Balance + destination.Balance", null)]
        public int Transfer(AccountAP source, AccountAP destination, int amount) 
        {
            source.Balance -= amount;
            destination.Balance += amount;
            //int transferredAmount = Math.Abs(source.Balance - destination.Balance);
            return amount;
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

   public class LinFuCalculator : ICalculator
    {
        
        public virtual int Mult(int arg1, int arg2)
        {
            
            return 0;
        }
    }

    public interface ICalculator
    {
        int Mult(int arg1, int arg2);
    }

    class LinFuCalculatorPoC : IPostcondition
    {
        #region IPostcondition Members

        public void BeforeMethodCall(object target, LinFu.DynamicProxy.InvocationInfo info)
        {
            
        }

        public bool Check(object target, LinFu.DynamicProxy.InvocationInfo info, object returnValue)
        {
            throw new NotImplementedException();
        }

        public void ShowError(System.IO.TextWriter output, object target, LinFu.DynamicProxy.InvocationInfo info, object returnValue)
        {
            throw new NotImplementedException();
        }

        #endregion

        #region IContractCheck Members

        public bool AppliesTo(object target, LinFu.DynamicProxy.InvocationInfo info)
        {
            throw new NotImplementedException();
        }

        public void Catch(Exception ex)
        {
            throw new NotImplementedException();
        }
        #endregion
    }


    class LinFuCalculatorPC : IPrecondition
    {
        #region IPrecondition Members

        public bool Check(object target, LinFu.DynamicProxy.InvocationInfo info)
        {
            LinFuCalculator cc = (LinFuCalculator)target;
            return true;
        }

        public void ShowError(System.IO.TextWriter output, object target, LinFu.DynamicProxy.InvocationInfo info)
        {
            throw new NotImplementedException();
        }

        #endregion

        #region IContractCheck Members

        public bool AppliesTo(object target, LinFu.DynamicProxy.InvocationInfo info)
        {
            if (info.TargetMethod.Name == "Mult")
                return true;
            return false;
        }

        public void Catch(Exception ex)
        {
            throw new NotImplementedException();
        }

        #endregion
    }

    class AccountPC : IPostcondition
    {
        private int oldBalance;
        public void BeforeMethodCall(object target, LinFu.DynamicProxy.InvocationInfo info)
        {
            oldBalance = (target as Account).Balance;
        }

        public bool Check(object target, LinFu.DynamicProxy.InvocationInfo info, object returnValue)
        {
            Account account = target as Account;
            return account.Balance == oldBalance + (int)info.Arguments[0];
        }

        public bool AppliesTo(object target, LinFu.DynamicProxy.InvocationInfo info)
        {
            if (target as Account == null)
                return false;
            if (info.TargetMethod.Name == "Deposit")
                return true;
            return false;
        }

        public void ShowError(System.IO.TextWriter output, object target, LinFu.DynamicProxy.InvocationInfo info, object returnValue)
        {
            throw new NotImplementedException();
        }


        public void Catch(Exception ex)
        {
            throw new NotImplementedException();
        }

    }


    class Test
    {
        
        public int member = 31;
    }

    public class Account
    {
        public int Balance;

        public virtual void Deposit(int amount)
        {
            this.Balance += amount;
        }
    }

    [AttributeUsage(AttributeTargets.All, AllowMultiple = false)]
    public class AsContractAttribute : Attribute
    {

        public AsContractAttribute(string preCondition, string postCondition,string t)
        {

        }
    }

  
}
