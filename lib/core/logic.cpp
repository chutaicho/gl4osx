//
//  Created by Takashi Aoki
//  http://takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#include "logic.h"

#pragma mark -
#pragma mark Event

core::EventManager* core::EventManager::instance = NULL;

void DispatchEvent(core::EventObj& event)
{	
	core::EventManager::getInstance()->dispatch(event);
};