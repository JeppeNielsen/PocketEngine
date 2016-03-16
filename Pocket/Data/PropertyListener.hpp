//
//  PropertyListener.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 13/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include <set>

namespace Pocket {
    template<class Context>
    class PropertyListener {
		public:
            template<class Value>
            void Add(Property<Value>& property, Context context, bool insert = true) {
            	property.Changed.Bind(this, &PropertyListener::PropertyChanged<Context>, context);
                if (insert) {
                    changedObjects.insert(context);
                }
            }
            template<class Value>
        	void Remove(Property<Value>& property, Context object) {
            	property.Changed.Unbind(this, &PropertyListener::PropertyChanged<Context>, object);
                auto it = changedObjects.find(object);
                if (it!=changedObjects.end()) {
                    changedObjects.erase(it);
                }
            }
        	using ChangedObjects = std::set<Context>;
            const ChangedObjects& Objects() { return changedObjects; }
        	void Clear() { changedObjects.clear(); }
        private:
        	template<class Owner>
        	void PropertyChanged(Context context) {
            	changedObjects.insert(context);
            }
			ChangedObjects changedObjects;
    };
}