//
//  PlatformBuilder.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once

namespace Pocket {
    class ProjectBuilder;
    class PlatformBuilder {
    protected:
        PlatformBuilder();
        virtual ~PlatformBuilder();

        ProjectBuilder* project;
        friend class ProjectBuilder;
    };
}
