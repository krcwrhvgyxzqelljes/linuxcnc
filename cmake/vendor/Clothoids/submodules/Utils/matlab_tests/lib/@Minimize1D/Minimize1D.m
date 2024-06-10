%-------------------------------------------------------+
%                                                       |
% Copyright (C) 2022                                    |
%                                                       |
%        , __                 , __                      |
%       /|/  \               /|/  \                     |
%        | __/ _   ,_         | __/ _   ,_              |
%        |   \|/  /  |  |   | |   \|/  /  |  |   |      |
%        |(__/|__/   |_/ \_/|/|(__/|__/   |_/ \_/|/     |
%                          /|                   /|      |
%                          \|                   \|      |
%                                                       |
%     Enrico Bertolazzi                                 |
%     Dipartimento di Ingegneria Industriale            |
%     Universita` degli Studi di Trento                 |
%     email: enrico.bertolazzi@unitn.it                 |
%                                                       |
%-------------------------------------------------------+
% Based on:
%
% - **G. E. Alefeld, Florian A Potra, Yixun Shi**,
%   *NUMERICAL RECIPES*,
%   ACM Transactions on Mathematical Software, vol 21, N.3, 1995
%
% Given a function f, and given a bracketing triplet of abscissas
% ax, bx, cx (such that bx is between ax and cx, and f(bx)
% is less than both f(ax) and f(cx)), this routine isolates
% the minimum to a fractional precision of about tol using
% Brent's method.
% The abscissa of the minimum is returned as xmin, and the
% minimum function value is returned as brent, the returned function value.
%
% Parameters: Maximum allowed number of iterations;
% golden ratio; and a small number that protects against
% trying to achieve fractional accuracy for a minimum that
% happens to be exactly zero.
%
classdef Minimize1D < matlab.mixin.Copyable

  properties (SetAccess = private, Hidden = true)
    m_fun;
    %--------------------
    m_num_fun_eval;
    m_num_iter_done;
    m_tolerance;
    %--------------------
    % Here GOLD is the default ratio by which successive intervals
    % are magnified and GLIMIT is the maximum magnification allowed
    % for a parabolic-fit step.
    m_GOLD;
    m_CGOLD;
    m_GLIMIT;
    m_TINY;
    %--------------------
    m_ax; m_bx; m_cx;
    m_fa; m_fb; m_fc;
    %--------------------
  end

  methods (Access = private, Hidden = true)
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function fx = f_evaluate( self, x )
      self.m_num_fun_eval = self.m_num_fun_eval+1;
      fx = feval( self.m_fun, x );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [fx,dfx] = fDf_evaluate( self, x )
      self.m_num_fun_eval = self.m_num_fun_eval+1;
      [fx,dfx] = feval( self.m_fun, x );
    end
  end

  methods
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function self = Minimize1D()
      self.m_num_iter_done = 0;
      self.m_num_fun_eval  = 0;
      self.m_tolerance     = 1e-8;
      self.m_GOLD          = 1.618034;
      self.m_CGOLD         = 0.3819660;
      self.m_GLIMIT        = 100;
      self.m_TINY          = 1e-20;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = num_fun_eval( self )
      res = self.m_num_fun_eval;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function res = num_iter_done( self )
      res = self.m_num_iter_done;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function set_tolerance( self, tol )
      self.m_tolerance = tol;
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    bracket( self, a, b )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    [ x, fx ] = golden( self )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    [ x, fx ] = brent( self )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    [ x, fx, dfx ] = dbrent( self )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [ x, fx ] = golden_search( self, a, b, FUN )
      self.m_fun = FUN;
      self.bracket( a, b );
      [ x, fx ] = self.golden();
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [ x, fx ] = brent_search( self, a, b, FUN )
      self.m_fun = FUN;
      self.bracket( a, b );
      [ x, fx ] = self.brent();
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [ x, fx ] = dbrent_search( self, a, b, FUN )
      self.m_fun = FUN;
      self.bracket( a, b );
      [ x, fx ] = self.dbrent();
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [ x, fx ] = eval( self, a, b, fun )
      self.m_fun = fun;
      [ x, fx ] = self.search( a, b );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    [x,y] = global_min( self, a, b, m, e )
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function [ x, fx ] = eval_global( self, a, b, fun )
      self.m_fun = fun;
      [ x, fx ] = self.global_min( a, b, 100, 1e-8 );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  end
end
