#ifndef CONSOLECOMMANDS_H
#define CONSOLECOMMANDS_H

#include "Core/Object.h"
#include "IWritableConsole.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class RZU_EXTERN ConsoleCommands : public Object {
	DECLARE_CLASSNAME(ConsoleCommands, 0)
public:
	typedef void (*CommandFunction)(IWritableConsole* console, const std::vector<std::string>& args);

	struct Command {
		const std::string name;
		const std::string alias;
		int minArgNum;
		int maxArgNum;
		std::string helpString;
		std::string usageExample;

		enum CallStatus { CS_Success, CS_NotEnoughArgs, CS_TooMuchArgs };
		CallStatus call(IWritableConsole* console, const std::vector<std::string>& args) const;
		virtual void operator()(IWritableConsole* console, const std::vector<std::string>& args) const = 0;
		virtual ~Command() {}

		Command(std::string name,
		        std::string alias,
		        int minArgNum,
		        int maxArgNum,
		        std::string helpString,
		        std::string usageExample)
		    : name(name),
		      alias(alias),
		      minArgNum(minArgNum),
		      maxArgNum(maxArgNum),
		      helpString(helpString),
		      usageExample(usageExample) {}
	};

	template<typename Lambda> struct CommandImpl : public Command {
		Lambda function;
		void operator()(IWritableConsole* console, const std::vector<std::string>& args) const override {
			function(console, args);
		}
		CommandImpl(std::string name,
		            std::string alias,
		            int minArgNum,
		            int maxArgNum,
		            std::string helpString,
		            std::string usageExample,
		            Lambda l)
		    : Command(name, alias, minArgNum, maxArgNum, helpString, usageExample), function(l) {}
	};

public:
	ConsoleCommands();

	template<typename Lambda>
	void addCommand(std::string name,
	                std::string alias,
	                int minArgNum,
	                int maxArgNum,
	                Lambda l,
	                std::string helpString,
	                std::string usageExample = std::string()) {
		Command* command = new CommandImpl<Lambda>(name, alias, minArgNum, maxArgNum, helpString, usageExample, l);
		addCommand(command);
	}

	void removeCommand(std::string name);

	const Command* getCommand(const std::string& name) const;

	static ConsoleCommands* get();

protected:
	static void commandHelp(IWritableConsole* console, const std::vector<std::string>& args);
	void addCommand(Command* command);

private:
	std::unordered_map<std::string, std::shared_ptr<Command>> commands;
};

#endif  // CONSOLECOMMANDS_H
