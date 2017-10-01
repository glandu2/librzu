#include "ConsoleCommands.h"
#include <map>

ConsoleCommands* ConsoleCommands::get() {
	static ConsoleCommands consoleCommands;

	return &consoleCommands;
}

ConsoleCommands::ConsoleCommands() {
	addCommand("console.help",
	           "help",
	           0,
	           1,
	           &commandHelp,
	           "Show help about commands",
	           "help : list all commands\r\n"
	           "help <cmd> : show command's help\r\n");
}

void ConsoleCommands::addCommand(Command command) {
	if(command.name.empty()) {
		log(LL_Error, "Can't register command without name, alias = %s\n", command.alias.c_str());
		return;
	}

	std::shared_ptr<Command> commandRef(new Command(command));
	bool addedAlias = false;

	auto result = commands.insert(std::make_pair(commandRef->name, commandRef));
	if(result.second == false) {
		log(LL_Error, "Can't register command, name already exists: %s\n", commandRef->name.c_str());
		return;
	}

	if(!commandRef->alias.empty()) {
		auto result = commands.insert(std::make_pair(commandRef->alias, commandRef));
		if(result.second == false)
			log(LL_Warning,
			    "Can't register alias %s for command %s\n",
			    commandRef->alias.c_str(),
			    commandRef->name.c_str());
		else
			addedAlias = true;
	}

	if(addedAlias)
		log(LL_Debug, "Registered command %s with alias %s\n", commandRef->name.c_str(), commandRef->alias.c_str());
	else
		log(LL_Debug, "Registered command %s\n", commandRef->name.c_str());
}

void ConsoleCommands::addCommand(std::string name,
                                 std::string alias,
                                 int minArgNum,
                                 int maxArgNum,
                                 CommandFunction function,
                                 std::string helpString,
                                 std::string usageExample) {
	Command command = {name, alias, minArgNum, maxArgNum, function, helpString, usageExample};
	addCommand(command);
}

void ConsoleCommands::addCommand(std::string name,
                                 int minArgNum,
                                 int maxArgNum,
                                 CommandFunction function,
                                 std::string helpString,
                                 std::string usageExample) {
	Command command = {name, std::string(), minArgNum, maxArgNum, function, helpString, usageExample};
	addCommand(command);
}

void ConsoleCommands::addCommand(std::string name,
                                 std::string alias,
                                 int argNum,
                                 CommandFunction function,
                                 std::string helpString,
                                 std::string usageExample) {
	Command command = {name, alias, argNum, argNum, function, helpString, usageExample};
	addCommand(command);
}

void ConsoleCommands::addCommand(
    std::string name, int argNum, CommandFunction function, std::string helpString, std::string usageExample) {
	Command command = {name, std::string(), argNum, argNum, function, helpString, usageExample};
	addCommand(command);
}

void ConsoleCommands::removeCommand(std::string name) {
	auto it = commands.find(name);
	if(it == commands.end()) {
		log(LL_Warning, "Can't remove command with name or alias %s, no such name\n", name.c_str());
		return;
	}

	std::shared_ptr<Command> command = it->second;

	commands.erase(command->name);
	if(!command->alias.empty())
		commands.erase(command->alias);

	if(!command->alias.empty())
		log(LL_Debug, "Removed command %s with alias %s\n", command->name.c_str(), command->alias.c_str());
	else
		log(LL_Debug, "Removed command %s\n", command->name.c_str());
}

const ConsoleCommands::Command* ConsoleCommands::getCommand(const std::string& name) const {
	auto it = commands.find(name);
	if(it != commands.end())
		return it->second.get();
	else
		return nullptr;
}

ConsoleCommands::Command::CallStatus ConsoleCommands::Command::call(IWritableConsole* console,
                                                                    const std::vector<std::string>& args) const {
	if((int) args.size() < minArgNum)
		return CS_NotEnoughArgs;
	else if((int) args.size() > maxArgNum)
		return CS_TooMuchArgs;

	(*function)(console, args);

	return CS_Success;
}

void ConsoleCommands::commandHelp(IWritableConsole* console, const std::vector<std::string>& args) {
	if(args.empty()) {
		std::map<std::string, std::shared_ptr<Command>> orderedCommands(get()->commands.begin(), get()->commands.end());
		int width = 0;
		auto itEnd = orderedCommands.end();

		// Compute max name size to align help strings
		for(auto it = orderedCommands.begin(); it != itEnd;) {
			Command* command = it->second.get();
			// Remove alias to avoid duplicate help lines
			if(it->first == command->name) {
				if(width < (int) command->name.size())
					width = (int) command->name.size();
				++it;
			} else {
				it = orderedCommands.erase(it);
			}
		}

		for(auto it = orderedCommands.begin(); it != itEnd; ++it) {
			Command* command = it->second.get();
			if(!command->alias.empty())
				console->writef("%-*s : %s (alias: %s)\r\n",
				                width,
				                command->name.c_str(),
				                command->helpString.c_str(),
				                command->alias.c_str());
			else
				console->writef("%-*s : %s\r\n", width, command->name.c_str(), command->helpString.c_str());
		}
	} else if(args.size() >= 1) {
		const Command* command = get()->getCommand(args[0]);
		if(!command) {
			console->writef("Command %s does not exists\r\n", args[0].c_str());
		} else {
			console->writef("%s\r\n%s\r\n", command->helpString.c_str(), command->usageExample.c_str());
		}
	}
}
