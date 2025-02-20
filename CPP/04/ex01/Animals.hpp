/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animals.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-cama <tde-cama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 10:41:35 by tde-cama          #+#    #+#             */
/*   Updated: 2021/09/17 18:05:32 by tde-cama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMAL_HPP
# define ANIMAL_HPP

# include <iostream>
# include "Brain.hpp"

class Animal
{
public:
	Animal(void);
	Animal(Animal const & rhs);
	virtual ~Animal(void);
	Animal & operator=(Animal const & rhs);

protected:
	std::string _type;
public:
	virtual void makeSound(void) const;
	std::string getType(void) const;

};

class Dog : public Animal
{
public:
	Dog(void);
	Dog(Dog const & rhs);
	virtual ~Dog(void);
	Dog & operator=(Dog const & rhs);
	virtual void makeSound(void) const;

	Brain* getBrain(void) const;
	std::string mindReader(void) const;

private:
	Brain* _brain;

};

class Cat : public Animal
{
public:
	Cat(void);
	Cat(Cat const & rhs);
	virtual ~Cat(void);
	Cat & operator=(Cat const & rhs);
	virtual void makeSound(void) const;

	Brain* getBrain(void) const;
	std::string mindReader(void) const;

private:
	Brain* _brain;
};

# endif
