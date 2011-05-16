using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TestApplication.Calculator;
using LinFu.DesignByContract2.Contracts;
using LinFu.DynamicProxy;
using LinFu.DesignByContract2.Core;

namespace TestApplication
{
    class Program
    {
        static void Main(string[] args)
        {
            AccountAP acc1 = new AccountAP();
            acc1.Balance = 500;
            AccountAP acc2 = new AccountAP();
            acc2.Balance = 300;
            AccountManager manager = new AccountManager();

            manager.Transfer(acc1, acc2, 100);
            /*
            Calculator.Calculator calculatorInstance = new TestApplication.Calculator.Calculator();
            calculatorInstance.Div(false, 2, 4096, "baba", "3", new Calculator.Calculator());
            //TestFalse(false);
            //TestTrue(true);
            Random r = new Random();
            for (int i = 0; i < 100; i++)
            {
                //Test(r.Next() % 2 == 0);
            }

            MsContractsCalculator cc = new MsContractsCalculator();
            //cc.Div(DateTime.Now.Second, 2);


            LinFuCalculator calculator = new LinFuCalculator();


            AdHocContract contract = new AdHocContract();
          //  contract.Preconditions.Add(new LinFuCalculatorPC());
            contract.Postconditions.Add(new AccountPC());
            ProxyFactory factory = new ProxyFactory();

            ContractChecker checker = new ContractChecker(contract);
            checker.Target = new Account();
            
            ICalculator wrapped = factory.CreateProxy<LinFuCalculator>(checker);
            Account account = factory.CreateProxy<Account>(checker);
            account.Deposit(200);
            



            // Require.On(contract).ForMethodWith(t => t.Name == "Mult").That<int>(arg => arg != 0).OtherwisePrint("Error");

            WrappedWorker worker = new WrappedWorker(new Worker());
            Worker wrappedWorker = factory.CreateProxy<Worker>(worker);
            wrappedWorker.Do();
            */
        }
    }
    public class Worker
    {
        public virtual void Do()
        {
            Console.WriteLine("Metoda Do()");
        }
    }

    public class WrappedWorker : IInvokeWrapper
    {
        object _target;
        public WrappedWorker(object target)
        {
            _target = target;
        }

        public void BeforeInvoke(InvocationInfo info)
        {
            Console.WriteLine("Przed wywołaniem metody Do()");
        }

        public object DoInvoke(InvocationInfo info)
        {
            Console.WriteLine("Wywołanie metody Do()");
            object result = null;
            // W celu wywołania oryginalnej metody Do() należy 
            // usunąć komentarz z następnej linii:
            result = info.TargetMethod.Invoke(_target, info.Arguments);
            return result;
        }

        public void AfterInvoke(InvocationInfo info, object returnValue)
        {
            Console.WriteLine("Po wywołaniu metody Do()");
        }
    }


}
