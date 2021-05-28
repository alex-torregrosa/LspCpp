#pragma once

#include "LibLsp/lsp/lsAny.h"
#include "lsWorkspaceClientCapabilites.h"
#include "lsTextDocumentClientCapabilities.h"


struct lsClientCapabilities {
  // Workspace specific client capabilities.
  boost::optional<lsWorkspaceClientCapabilites> workspace;

  // Text document specific client capabilities.
  boost::optional<lsTextDocumentClientCapabilities> textDocument;

  /**
	* Window specific client capabilities.
  */
  boost::optional<lsp::Any>  window;
  /**
   * Experimental client capabilities.
   */
  boost::optional<lsp::Any>  experimental;

  MAKE_SWAP_METHOD(lsClientCapabilities, workspace, textDocument, window, experimental);
};
MAKE_REFLECT_STRUCT(lsClientCapabilities, workspace, textDocument, window, experimental);



