//
//  PropertyListener.h
//  Homeland
//
//  Created by Jeppe Nielsen on 15/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include <set>

namespace Pocket {
    template<class Context>
    class PropertyListener {
		public:
            template<class Owner, class Value>
            void Add(Property<Owner, Value>& property, Context context, bool insert = true) {
            	property.Changed += event_handler(this, &PropertyListener::PropertyChanged<Owner>, context);
                if (insert) {
                    changedObjects.insert(context);
                }
            }
            template<class Owner, class T>
        	void Remove(Property<Owner, T>& property, Context object) {
            	property.Changed -= event_handler(this, &PropertyListener::PropertyChanged<Owner>, object);
                auto it = changedObjects.find(object);
                if (it!=changedObjects.end()) {
                    changedObjects.erase(it);
                }
            }
        	typedef std::set<Context> ChangedObjects;
            const ChangedObjects& Objects() { return changedObjects; }
        	void Clear() { changedObjects.clear(); }
        private:
        	template<class Owner>
        	void PropertyChanged(Owner owner, Context context) {
            	changedObjects.insert(context);
            }
			ChangedObjects changedObjects;
    };
}