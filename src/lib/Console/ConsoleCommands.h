#ifndef CONSOLECOMMANDS_H
#define CONSOLECOMMANDS_H

#include "Core/Object.h"
#include "IWritableConsole.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class RZU_EXTERN ConsoleCommands : public Object {
	// DECLARE_CLASS(ConsoleCommands)
public:
	typedef void (*CommandFunction)(IWritableConsole* console, const std::vector<std::string>& args);
	struct Command {
		const std::string name;
		const std::string alias;
		int minArgNum;
		int maxArgNum;
		CommandFunction function;
		std::string helpString;
		std::string usageExample;

		enum CallStatus { CS_Success, CS_NotEnoughArgs, CS_TooMuchArgs };
		CallStatus call(IWritableConsole* console, const std::vector<std::string>& args) const;
	};

public:
	ConsoleCommands();

	void addCommand(Command command);
	void addCommand(std::string name,
	                std::string alias,
	                int minArgNum,
	                int maxArgNum,
	                CommandFunction function,
	                std::string helpString,
	                std::string usageExample = std::string());
	void addCommand(std::string name,
	                int minArgNum,
	                int maxArgNum,
	                CommandFunction function,
	                std::string helpString,
	                std::string usageExample = std::string());
	void addCommand(std::string name,
	                std::string alias,
	                int argNum,
	                CommandFunction function,
	                std::string helpString,
	                std::string usageExample = std::string());
	void addCommand(std::string name,
	                int argNum,
	                CommandFunction function,
	                std::string helpString,
	                std::string usageExample = std::string());

	void removeCommand(std::string name);

	const Command* getCommand(const std::string& name) const;

	static ConsoleCommands* get();

protected:
	static void commandHelp(IWritableConsole* console, const std::vector<std::string>& args);

private:
	std::unordered_map<std::string, std::shared_ptr<Command>> commands;
};

#endif  // CONSOLECOMMANDS_H
