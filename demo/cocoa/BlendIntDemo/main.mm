//
//  main.m
//  BlendIntDemo
//
//  Created by Freeman Zhang on 14-1-5.
//  Copyright (c) 2014年 Freeman Zhang. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include <BlendInt/Interface.hpp>

int main(int argc, const char * argv[])
{
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;
        
    return NSApplicationMain(argc, argv);
}
