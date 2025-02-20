/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-cama <tde-cama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 11:55:35 by tde-cama          #+#    #+#             */
/*   Updated: 2022/01/08 11:40:55 by tde-cama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <server.hpp>
#include <poll.hpp>
#include <accept.hpp>
#include <receive.hpp>
#include <requested_file.hpp>
#include <response.hpp>
#include <send.hpp>

Server serv;

void	sig_handle(int signus){
    for(size_t i = 0; i < serv.getSize(); ++i)
        close(serv.getSock(i));
    exit(signus);
};

int main(int argc, char* argv[])
{
    std::string config_dir("./configs/");
    struct stat file_stat;
    
    stat(config_dir.c_str(), &file_stat);
    if(!S_ISDIR(file_stat.st_mode)){
        std::cout << "The configs dir is missing :(" << std::endl;
        return 1;
    }
    
    if(argc > 2){
        std::cout << "Too many arguments" << std::endl;
        return 2;
    }
    
    std::string config_path;
    if(argc > 1){
        config_path = argv[1];
        std::string suff(".conf");
        if(config_path.find("/") == std::string::npos)
            config_path.insert(0, config_dir);
        if(config_path.find(suff) == std::string::npos)
            config_path.append(suff);
    }
    else
        config_path = "./configs/default.conf";
    
    try
    {
        serv.start(config_path);

        while (1)
        {
            signal(SIGINT, &sig_handle);
            std::cout << "\n+++ Waiting for new connection +++\n" << std::endl;
            Poll poll(serv);
            switch (poll.status()){
                case 0:
                    std::cout << "\n* Poll Timed Out *\n" << std::endl;
                case -1:
                    continue ;
                default:
                    break;
            }
            size_t i = poll.getSelected();
            Accept  a(serv.getSock(i), serv.getBind(i));
            Receive recv(a.getSock());
            switch (recv.getStatus()){
                case 0:
                    std::cout << "\n* Connection was closed by the Client *\n" << std::endl;
                case -1:
                    close(a.getSock());
                    continue ;
                default:
                    break;
            }
            Req_Parser req(recv);
            size_t j = serv.select(i, req.getHost());
            Config conf_l = serv[i].at(j).select(req.getFile());
            Req_File file(conf_l, req);
            Response res(file, req);
            std::cout << recv.bin() << std::endl;
            Send send(a, res, file);
        }
        
    }
    catch(std::exception & e)
    {
        std::cout << e.what() << std::endl;
        return 2;
    }
    return 0;
};