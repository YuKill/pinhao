/*-------------------------- PINHAO project --------------------------*/

/**
 * @file Formula.h
 */

#ifndef PINHAO_FORMULA_H
#define PINHAO_FORMULA_H

namespace pinhao {

  class FormulaBase {
    public:
      enum class FormulaKind {
        Literal = 0, 
        BooleanBinOp, 
        ArithmeticBinOp, 
        If,
        Feature
      };

      static const std::vector<std::string> FormulaKindString = {
        "literal", "bool-binop", "arith-binop", "if", "feature"
      }; 

      FormulaKind getFormulaKind(std::string String) {
        int Count = 0;
        for (auto S : FormulaKindString) {
          if (S == String) 
            return static_cast<FormulaKind>(Count);
          ++Count;
        }
        assert(false && "There is no such FormulaKindString.");
      }

      std::string getFormulaKindString(FormulaKind K) {
        return FormulaKindString[static_cast<int>(K)]; 
      }

      static std::unique_ptr<FormulaBase> create(FormulaKind K, ValueType Type) {
        switch (K) {
          case FormulaKind::Literal: 
          case FormulaKind::BooleanBinOp: 
          case FormulaKind::ArithmeticBinOp: 
          case FormulaKind::If: 
          case FormulaKind::Feature: 
        } 
      }

    protected:
      template <class T>
        static T getFormulaValue(const FormulaBase *FB) {
          Formula<T> *CastFormula = static_cast<const Formula<T>*>(FB);
          return CastFormula->getValue();
        }

      virtual void appendImpl(YAML::Emitter &Emitter) = 0;
      virtual void getImpl(const YAML::Node &Node) = 0;

    public:
      virtual void solveFor(FeatureSet *Set) = 0;
      virtual ValueType getType() = 0;
      virtual FormulaKind getKind() = 0;

      void appendYaml(YAML::Emitter &Emitter) {
        appendImpl(Emitter);
      }

      void print(std::ostream &Out = std::cout) {
        YAML::Emitter Emitter(Out);
        appendYaml(Emitter);
        Out << std::endl;
      }

      void get(const YAML::Node &Node) { getImpl(Node); }

  };

  template <class T>
    class Formula : public FormulaBase {
      protected:
        T Value;

      public:
        T &getValue() { return Value; }

    };

  template <class T, class OpT = T>
    class BinOpFormulaBase : public Formula<T> {
      protected:
        class enum OperatorKind {
          PLUS = 0, MIN, MUL, DIV, EQ, NEQ, LT, GT, LEQ, GEQ
        };

        OperatorKind Operator;

        T doOperation(OpT One, OpT Two) = 0;

      public:
        std::unique_ptr<Formula<OpT>> Lhs;
        std::unique_ptr<Formula<OpT>> Rhs;

        bool isArithmeticBinOp() { return getKind() == FormulaKind::ArithmeticBinOp; }
        bool isBooleanBinOp() { return getKind() == FormulaKind::BooleanBinOp; }
        void setOperator(int NewOperator) { Operator = static_cast<OperatorKind>(NewOperator); }

        void solveFor(FeatureSet *Set) override {
          Lhs->solveFor(Set); 
          Rhs->solveFor(Set); 
          Value = doOperation(Lhs->getValue(), Rhs->getValue());
        }

    };

  template <class T>
    class ArithmeticBinOpFormula : BinOpFormulaBase<T, T> {
      protected:
        T doOperation(T One, T Two) override {
          switch (Operator) {
            case Operator::PLUS: return One + Two;
            case Operator::MIN:  return One - Two;
            case Operator::MUL:  return One * Two; 
            case Operator::DIV:  return One / Two; 
            default: return 0;
          }
        }

      public:
        ValueType getType() override { return getValueTypeFor<T>(); }
        FormulaKind getKind() override { return FormulaKind::ArithmeticBinOp; }

    };

  template <class T>
    class BooleanBinOpFormula : BinOpFormulaBase<bool, T> {
      protected:
        bool doOperation(T One, T Two) override {
          int OperatorId = static_cast<int>(Operator) + static_cast<int>(OperatorKind::DIV);
          switch (static_cast<OperatorKind>(OperatorId)) {
            case Operator::EQ:   return One == Two; 
            case Operator::NEQ:  return One != Two; 
            case Operator::LT:   return One < Two; 
            case Operator::GT:   return One > Two; 
            case Operator::LEQ:  return One <= Two; 
            case Operator::GEQ:  return One >= Two; 
            default: return false;
          }
        }

      public:
        ValueType getType() override { return getValueTypeFor<bool>(); }
        FormulaKind getKind() override { return FormulaKind::BooleanBinOp; }

    };

  template <class T>
    class IfFormula : Formula<T> {
      public:
        std::unique_ptr<Formula<bool>> Condition;
        std::unique_ptr<Formula<T>> ThenBody;
        std::unique_ptr<Formula<T>> ElseBody;

        void solveFor(FeatureSet *Set) override {
          Condition->solveFor(Set);   
          if (Condition->getValue()) {
            ThenBody->solveFor(Set);
            Value = ThenBody->getValue();
          } else {
            ElseBody->solveFor(Set);
            Value = ElseBody->getValue();
          }
        }

        ValueType getType() override { return getValueTypeFor<T>(); }
        FormulaKind getKind() override { return FormulaKind::If; }
    };

  template <class T>
    class LitFormula : Formula<T> {
      public:
        void setValue(T Value) { this->Value = Value; }
        ValueType getType() override { return getValueTypeFor<T>(); }
        FormulaKind getKind() override { return FormulaKind::Literal; }

        void solveFor(FeatureSet *Set) override {}
    };

  template <class T>
    class FeatureFormula : Formula<T> {
      public:
        std::pair<std::string, std::string> FeaturePair;

        ValueType getType() override { return getValueTypeFor<T>(); }
        FormulaKind getKind() override { return FormulaKind::Feature; }

        void solveFor(FeatureSet *Set) override {
          Value = Set->getFeature<T>(FeaturePair); 
        }
    };

}

#endif
