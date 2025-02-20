/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-cama <tde-cama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 14:32:15 by tde-cama          #+#    #+#             */
/*   Updated: 2022/01/08 11:44:12 by tde-cama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.hpp>

Server::Server(void){
    return ;
};

void Server::start(std::string const & path){
    std::ifstream	ifs(path.c_str());
    if (!ifs)
        throw Server::FdFailedException();
    checkSyntax(ifs);

    std::string line;
    size_t option ;
    
    while (std::getline(ifs, line, '{')){
    	if ((option = line.rfind("server")) < std::string::npos){
            if ( (option + 5) == line.find_last_not_of(" \t\n")
            && option == line.find_first_not_of(" \t\n") ){
                Config conf(ifs);
                
                size_t i = 0;
                for(; i < _servers.size(); ++i){
                    if( conf.getHost() == _servers[i].at(0).getHost()
                    && conf.getPort() == _servers[i].at(0).getPort() ){
                        _servers[i].push_back(conf);
                        break ;
                    }
                }
                if(i == _servers.size()){
                    Socket  s;
                    Bind    b(s.getSock(), conf);
                    Listen  l(s.getSock());
                    std::vector<Config> vC;
                    vC.push_back(conf);
                    _servers.push_back(vC);
                    _sockets.push_back(s);
                    _binds.push_back(b);
                }
            }
            else 
                throw Server::SyntaxErrorException();
        }
    }
    ifs.close();

};

Server::~Server(void){
    return ;
};

Server::Server(Server const & src){
    *this = src;
};

Server & Server::operator=(Server const & rhs){

    if (this != &rhs){
        this->_servers = rhs._servers;
        this->_sockets = rhs._sockets;
        this->_binds = rhs._binds;
    }
    
    return *this;
};

const char* Server::FdFailedException::what() const throw(){
    return "configuration file was not found";
};

const char* Server::SyntaxErrorException::what() const throw(){
    return "configuration file could not be read due to a syntax error";
};

 std::vector<Config> & Server::operator[](size_t const & i){
    if(i >= _servers.size())
       throw Server::FdFailedException();
    return _servers[i];
};

size_t Server::getSize() const{
    return _servers.size();
};

int Server::getSock(size_t const & i) const{
    return _sockets.at(i).getSock();
};

Bind Server::getBind(size_t const & i) const{
    return _binds.at(i);
};

size_t Server::select(size_t const & i, std::string const & host) const{

    size_t j = 0;
    size_t selected = 0;
    size_t max = 0;
    for(; j < _servers[i].size(); ++j){
        std::string name = _servers[i].at(j).getServerName();
        if (name == host)
            return j % _servers[i].size();
        if (name.find("*", 0, 1) == 0){
            name.erase(0, 1);
            if(host.rfind(name.c_str(), host.length() - 1, name.length()) < std::string::npos){
                if(name.length() > max){
                    selected = j;
                    max = name.length();
                }
            }
        }
        else if (name.rfind("*", name.length() - 1, 1) ==  name.length() - 1){
            name.erase(name.length() - 1, 1);
            if(host.rfind(name.c_str(), 0, name.length()) < std::string::npos){
                if(name.length() > max){
                    selected = j;
                    max = name.length();
                }
            }
        }
    }
    return selected % _servers[i].size();
};

void Server::checkSyntax(std::ifstream  & ifs) const{
    std::string line;
    ssize_t bra = 0;
    while (std::getline(ifs, line)){
        size_t pos = 0;
        while( (pos = line.find("{", pos) ) < std::string::npos ){
            ++bra;
            ++pos;
        }
        pos = 0;
        while( (pos = line.find("}", pos) ) < std::string::npos ){
            --bra;
            ++pos;
            if (bra < 0)
                throw Server::SyntaxErrorException();
        }
        if( (bra == 0 && line.find("}") < std::string::npos)
        && line.find(";") == std::string::npos)
            throw Server::SyntaxErrorException();
        if(line.find_first_not_of(" \n\t") < std::string::npos
        && line.find_last_of("{};") == std::string::npos)
            throw Server::SyntaxErrorException();
    }
    if (bra > 0)
        throw Server::SyntaxErrorException();
    ifs.clear();
    ifs.seekg(std::ios_base::beg);
    
};
