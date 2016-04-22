/*-------------------------- PINHAO project --------------------------*/

/**
 * @file YamlOptions.h
 */

#ifndef PINHAO_YAML_OPTIONS_H
#define PINHAO_YAML_OPTIONS_H

#include "yaml-cpp/yaml.h"

namespace pinhao {

  namespace config {

    /// @brief Returns true if there is already an option with this name.
    bool isRegistered(std::string OptName); 

    /** 
     * @brief Parse an @a YAML::Node.
     * @details 
     * It parses the @a YAML::Node by looking at all options registered. It will
     * fill these variables with its value parsed.
     *
     * It is a friend of the class @a YamlOptBase, so that it can use the function @a parse
     * which is protected.
     */
    void parseOptions(YAML::Node &Node);

    /** 
     * @brief Parse a file which name is @a Filename.
     * @details 
     * It parses the file @a Filename by looking at all options registered. It will
     * fill these variables with its value parsed.
     */
    void parseOptions(std::string Filename);

    /// @brief Prints all the options registered.
    void printHelp(std::ostream &Out);

    /**
     * @brief The base class for the creation of yaml options.
     * @details
     * When it is constructed, it registers itself, and when destroyed, it unregisters
     * itself.
     */
    class YamlOptBase {
      private:
        std::string Name;
        std::string Description;
        bool Required;

      protected:
        bool Parsed;

        /// @brief Register this option.
        void registerOption();

        /// @brief Unregister this option.
        void unregister(); 

      public:
        virtual ~YamlOptBase(); 

        YamlOptBase(std::string Name, std::string Description, bool Required); 

        /// @brief Gets the name of the option.
        std::string getName();

        /// @brief Returns true if this option is required.
        bool isRequired(); 

        /// @brief Returns true if this option has parsed a @a YAML::Node already.
        bool hasParsed();

        /**
         * @brief Parses a @a YAML::Node.
         * @details
         * Fills in the value that was gotten by the parse.
         */
        virtual void parse(const YAML::Node &Node) = 0;

    };

    /**
     * @brief A template class for the registration of yaml options.
     */
    template <class ValueType>
      class YamlOpt : public YamlOptBase {
        private:
          ValueType Value;

        public:
          YamlOpt(std::string Name, std::string Description, bool Required) : 
            YamlOptBase(Name, Description, Required) {}

          YamlOpt(std::string Name, std::string Description, bool Required, ValueType DefaultValue) : 
            YamlOpt(Name, Description, Required) {
              Value = DefaultValue; 
            }

          /// @brief Gets a reference to @a Value associated with this class.
          ValueType &get() { return Value; }

          void parse(const YAML::Node &Node) override {
            Value = Node.as<ValueType>();
            Parsed = true;
          }

      };

  } // namespace config

} // namespace pinhao

#endif
